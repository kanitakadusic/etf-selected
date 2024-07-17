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

insert into TABELAB (id, cijelibroj, fktabelaa)
             values (1, 1, 1);
insert into TABELAB (id, cijelibroj, fktabelaa)
             values (2, 3, 1);
insert into TABELAB (id, cijelibroj, fktabelaa)
             values (3, 6, 2);
insert into TABELAB (id, cijelibroj, fktabelaa)
             values (4, 11, 2);
insert into TABELAB (id, cijelibroj, fktabelaa)
             values (5, 22, 3);

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

--1.
-- izvrsenje: uspjesno
insert into TABELAA (id, naziv, datum, cijelibroj, realnibroj)
             values (6, 'tekst', null, null, 6.20);

--2.
-- izvrsenje: neuspjesno
-- razlog: unique constraint violated (cijelibroj)
-- objasnjenje: pokusava se unijeti slog s vrijednoscu kolone * cijelibroj = 1 * pri cemu je navedena vrijednost kolone vec prisutna
insert into TABELAB (id, naziv, datum, cijelibroj, realnibroj, fktabelaa)
             values (6, null, null, 1, null, 1);

--3.
-- izvrsenje: uspjesno
insert into TABELAB (id, naziv, datum, cijelibroj, realnibroj, fktabelaa)
             values (7, null, null, 123, null, 6);

--4.
-- izvrsenje: uspjesno
insert into TABELAC (id, naziv, datum, cijelibroj, realnibroj, fktabelab)
             values (4, 'NO', null, 55, null, null);

--5.
-- izvrsenje: uspjesno
update TABELAA
set naziv = 'tekst'
where naziv is null and
      cijelibroj is not null;

--6.
-- izvrsenje: neuspjesno
-- razlog: foreign key constraint violated (fktabelab -> id)
-- objasnjenje: brisanjem tabele TABELAB bi podaci tabele TABELAC(fktabelab) ostali u neispravnom stanju
drop table TABELAB;

--7.
-- izvrsenje: neuspjesno
-- razlog: foreign key constraint violated (fktabelaa -> id)
-- objasnjenje: doslo bi do brisanja sloga tabele TABELAA sa * id = 3 * cija se vrijednost koristi kao strani kljuc u tabeli TABELAB
delete from TABELAA
where realnibroj is null;

--8.
-- izvrsenje: uspjesno
delete from TABELAA
where id = 5;

--9.
-- izvrsenje: uspjesno
update TABELAB
set fktabelaa = 4
where fktabelaa = 2;

--10.
-- izvrsenje: uspjesno
alter table TABELAA
add constraint cst
check (naziv like 'tekst');
