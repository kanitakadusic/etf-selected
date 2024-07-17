drop table TABELAC;
drop table TABELAB;
drop table TABELAA;

--drop table TABELAABEKAP;
--drop sequence BROJACB;
--drop sequence SEQ;

create table TABELAA (
    id         number,
    naziv      varchar2(100),
    datum      date,
    cijelibroj number,
    realnibroj number,

    constraint a_id_pk        primary key (id),
    constraint a_cijelibroj_c check       (cijelibroj not between 5 and 15),
    constraint a_realnibroj_c check       (realnibroj > 5)
);

insert into TABELAA (id, naziv, realnibroj)
             values (1, 'tekst', 6.2);
insert into TABELAA (id, cijelibroj, realnibroj)
             values (2, 3, 5.26);
insert into TABELAA (id, naziv, cijelibroj)
             values (3, 'tekst', 1);
insert into TABELAA (id)
             values (4);
insert into TABELAA (id, naziv, cijelibroj, realnibroj)
             values (5, 'tekst', 16, 6.78);

create table TABELAB (
    id         number,
    naziv      varchar2(100),
    datum      date,
    cijelibroj number,
    realnibroj number,
    fktabelaa  number not null,

    constraint b_id_pk             primary key (id),
    constraint b_cijelibroj_u      unique      (cijelibroj),
    constraint b_fktabelaa_a_id_fk foreign key (fktabelaa) references tabelaa (id)
);

create sequence BROJACB
start with 1;

insert into TABELAB (id, cijelibroj, fktabelaa)
             values (BROJACB.nextval, 1, 1);
insert into TABELAB (id, cijelibroj, fktabelaa)
             values (BROJACB.nextval, 3, 1);
insert into TABELAB (id, cijelibroj, fktabelaa)
             values (BROJACB.nextval, 6, 2);
insert into TABELAB (id, cijelibroj, fktabelaa)
             values (BROJACB.nextval, 11, 2);
insert into TABELAB (id, cijelibroj, fktabelaa)
             values (BROJACB.nextval, 22, 3);

create table TABELAC (
    id         number,
    naziv      char(3) not null,
    datum      date,
    cijelibroj number not null,
    realnibroj number,
    fktabelab  number,

    constraint c_id_pk primary key (id),
    constraint fkcnst  foreign key (fktabelab) references tabelab (id)
);

insert into TABELAC (id, naziv, cijelibroj, fktabelab)
             values (1, 'YES', 33, 4);
insert into TABELAC (id, naziv, cijelibroj, fktabelab)
             values (2, 'NO', 33, 2);
insert into TABELAC (id, naziv, cijelibroj, fktabelab)
             values (3, 'NO', 55, 1);

-- - - - - - - - - - - - - - - - - - - - -

create or replace trigger t1
after insert on TABELAB
for each row
    declare
        v_granica constant number := 50;
        v_up      constant number := 1.25;
        v_down    constant number := 0.75;
    begin
        if :new.cijelibroj < v_granica then
            update TABELAA a
            set a.realnibroj = v_up * a.realnibroj
            where a.id = :new.fktabelaa;
        else
            update TABELAA a
            set a.realnibroj = v_down * a.realnibroj
            where a.id = :new.fktabelaa;
        end if;
    end;
/

create or replace trigger t2
before insert or update on TABELAC
for each row
    declare
        v_datum      constant date := sysdate;
        v_cijelibroj TABELAB.cijelibroj%type;
        v_faktor     constant number := 2;
        v_fktabelaa  TABELAB.id%type;
    begin
        select max(b.cijelibroj)
        into v_cijelibroj
        from TABELAB b;

        v_cijelibroj := v_faktor * v_cijelibroj;

		select b.fktabelaa
        into v_fktabelaa
        from TABELAB b
        where b.id = :new.fktabelab;

        insert into TABELAB (id, datum, cijelibroj, fktabelaa)
                     values (BROJACB.nextval, v_datum, v_cijelibroj, v_fktabelaa);
    end;
/

create table TABELAABEKAP (
    id          number,
    naziv       varchar2(100),
    datum       date,
    cijelibroj  number,
    realnibroj  number,
    cijelibrojb number,
    sekvenca    number,

    constraint p_id_pk        primary key (id),
    constraint p_cijelibroj_c check       (cijelibroj not between 5 and 15),
    constraint p_realnibroj_c check       (realnibroj > 5)
);

create sequence SEQ
increment by 1
start with 1
maxvalue 10
minvalue 1
cycle
nocache;

create or replace trigger t3
after insert on TABELAB
for each row
    begin
        update TABELAABEKAP p
        set p.cijelibrojb = p.cijelibrojb + :new.cijelibroj
        where p.id = (select a.id
                      from TABELAA a
                      where a.id = :new.fktabelaa);

        if (SQL%NOTFOUND) then
            insert into TABELAABEKAP (id, naziv, datum, cijelibroj, realnibroj, cijelibrojb, sekvenca)
            select a.id, a.naziv, a.datum, a.cijelibroj, a.realnibroj, :new.cijelibroj, SEQ.nextval
            from TABELAA a
            where a.id = :new.fktabelaa;
        end if;
    end;
/

-- - - - - - - - - - - - - - - - - - - - -

insert into TABELAB (id, naziv, datum, cijelibroj, realnibroj, fktabelaa)
             values (BROJACB.nextval, null, null, 2, null, 1);

insert into TABELAB (id, naziv, datum, cijelibroj, realnibroj, fktabelaa)
             values (BROJACB.nextval, null, null, 4, null, 2);

insert into TABELAB (id, naziv, datum, cijelibroj, realnibroj, fktabelaa)
             values (BROJACB.nextval, null, null, 8, null, 1);

insert into TABELAC (id, naziv, datum, cijelibroj, realnibroj, fktabelab)
             values (4, 'NO', null, 5, null, 3);

insert into TABELAC (id, naziv, datum, cijelibroj, realnibroj, fktabelab)
             values (5, 'YES', null, 7, null, 3);

insert into TABELAC (id, naziv, datum, cijelibroj, realnibroj, fktabelab)
             values (6, 'NO', null, 9, null, 2);

UPDATE TABELAC
SET cijelibroj = 10
WHERE id = 2;

delete from TABELAB
where id not in (select fktabelab from TABELAC);

delete from TABELAA
where id in (3, 4, 6);

-- - - - - - - - - - - - - - - - - - - - -

--select sum(id * 3 + cijelibrojb * 3) from TABELAABEKAP; --2031
--select sum(id * 3 + cijelibroj * 3) from TABELAC; --420
--select sum(mod(id, 10) * 3) from TABELAB; --30
--select sum(id + realnibroj) * 10 from TABELAA; --264 (263.9328125)

select sum(id * 7 + cijelibrojb * 7) from TABELAABEKAP; --4739
select sum(id * 7 + cijelibroj * 7) from TABELAC; --980
select sum(mod(id, 10) * 7) from TABELAB; --70
select sum(id * 5 + realnibroj) * 10 from TABELAA; --584 (583.9328125)
