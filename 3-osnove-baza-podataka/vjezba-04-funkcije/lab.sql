SELECT * FROM employees;
SELECT * FROM departments;
SELECT * FROM locations;
SELECT * FROM countries;
SELECT * FROM regions;
SELECT * FROM jobs;

-- ZADATAK 1

SELECT SYSDATE "date",
       USER "user"
FROM dual;

-- ZADATAK 2

SELECT employee_id,
       first_name,
       last_name,
       salary,
       Round(salary * 1.25) "Plata uvecana za 25%"
FROM employees;

-- ZADATAK 3

SELECT employee_id,
       first_name,
       last_name,
       salary,
       Round(salary * 1.25) "Plata uvecana za 25%",
       Mod(Round(salary * 1.25), 100) "Ostatak plate"
FROM employees;

-- ZADATAK 4

SELECT first_name || ' ' || last_name "Zaposlenik",
       To_Char(hire_date, 'DY - MON, YYYY') "Datum zaposlenja",
       To_Char(Next_Day(Add_Months(hire_date, 6), 'MONDAY'), 'DY - MON, YYYY') "Ponedjeljak nakon 6 mjeseci"
FROM employees;

-- ZADATAK 5

SELECT e.first_name || ' ' || e.last_name "Zaposlenik",
       d.department_name "Odjel",
       r.region_name "Kontinent",
       Round(Months_Between(SYSDATE, e.hire_date)) "Broj mjeseci zaposlenja"
FROM employees e, departments d, locations l, countries c, regions r
WHERE e.department_id = d.department_id(+) AND
      d.location_id = l.location_id(+) AND
      l.country_id = c.country_id(+) AND
      c.region_id = r.region_id(+)
ORDER BY e.first_name, e.last_name;

-- ZADATAK 6

SELECT first_name || ' ' ||
       last_name || ' prima platu ' ||
       salary || ' mjesecno, ali on bi zelio platu ' ||
       salary * (1 + Nvl(commission_pct, 0)) * 4.5 || '.' "NEKI IZRACUN"
FROM employees
WHERE department_id IN (10, 30, 50);

-- ZADATAK 7

SELECT LPad(first_name || ' ' || last_name || ' + ' || salary, 50, '$') "Zaposlenik i plata"
FROM employees;

-- ZADATAK 8

SELECT Concat(Lower(SubStr(first_name, 1, 1)), Upper(SubStr(first_name, 2))) || ' ' ||
       Concat(Lower(SubStr(last_name, 1, 1)), Upper(SubStr(last_name, 2))) "Ime i prezime zaposlenog",
       Length(Concat(first_name, last_name)) "Duzina imena i prezimena"
FROM employees
WHERE SubStr(first_name, 1, 1) IN ('A', 'J', 'M', 'S');

-- ZADATAK 9

SELECT first_name || ' ' || last_name "Zaposlenik",
       hire_date "Datum zaposlenja",
       To_Char(hire_date, 'DAY') "Prvi radni dan",
       (8 - (Next_Day(hire_date, 'MONDAY') - hire_date)) "Redni broj dana u sedmici"
FROM employees
ORDER BY (Next_Day(hire_date, 'MONDAY') - hire_date) DESC;

-- ZADATAK 10

SELECT e.first_name || ' ' || e.last_name "Zaposlenik",
       Nvl(l.city, '?') "Grad",
       Decode(e.commission_pct,
              NULL, 'Zaposlenik ne prima dodatak na platu.',
              e.salary * Nvl(e.commission_pct, 0)) "Dodatak na platu"
FROM employees e, departments d, locations l
WHERE e.department_id = d.department_id(+) AND
      d.location_id = l.location_id(+)
ORDER BY e.first_name, e.last_name;

-- ZADATAK 11

SELECT first_name || ' ' || last_name "Zaposlenik",
       salary "Plata",
       REPLACE(SubStr(To_Char(Power(10, Round(salary / 1000))), 2), 0, '*') "Indikator plate"
FROM employees
ORDER BY first_name, last_name;

-- ZADATAK 12

SELECT e.first_name || ' ' || e.last_name "Zaposlenik",
       j.job_title "Posao",
       Decode(Lower(j.job_title),
              'president', 'A',
              'manager', 'B',
              'analyst', 'C',
              'sales manager', 'D',
              'programmer', 'E',
              'X') "Stepen"
FROM employees e, jobs j
WHERE e.job_id = j.job_id
ORDER BY e.first_name, e.last_name;

-- OSTALO

SELECT first_name, hire_date, Months_Between(SYSDATE, hire_date), Round(Months_Between(SYSDATE, hire_date)), Add_Months(hire_date, -6)
FROM employees;

SELECT Last_Day(Add_Months(SYSDATE, 1)), Next_Day(SYSDATE, 3), Next_Day(SYSDATE, 'Friday')
FROM dual;

SELECT SYSDATE, Round(SYSDATE), Trunc(SYSDATE), Round(SYSDATE, 'dd'), Trunc(SYSDATE, 'dd'), Round(SYSDATE, 'mm'), Trunc(SYSDATE, 'mm'), Round(SYSDATE, 'yyyy'), Trunc(SYSDATE, 'yyyy')
FROM dual;

SELECT Abs(-234.123), Ceil(12.45), Floor(12.45), Round(Sin(3.14 / 2)), Exp(1), Ln(Exp(2)), Log(2, 8), Mod(9, 5), Power(2, 3)
FROM dual;

SELECT To_Char(1.23), To_Char(SYSDATE, 'dd.mm.yyyy.'), To_Char(22.123456789 , 'B9999999')
FROM dual;

SELECT first_name || ' ' || last_name, replace(first_name || ' ' || last_name, 'le')
FROM employees;
