SELECT * FROM employees;

SELECT first_name || ' ' || last_name "IME I PREZIME",
       salary AS plata
FROM employees
WHERE salary > 2345;

SELECT first_name || ' ' || last_name "IME I PREZIME",
       department_id "SIFRA ODJELA"
FROM employees
WHERE employee_id = 102;

SELECT *
FROM employees
WHERE salary < 2600 OR
      salary > 10000;

SELECT *
FROM employees
WHERE salary NOT BETWEEN 2600 AND 10000;

SELECT first_name || ' ' || last_name "Zaposleni",
       job_id "Posao",
       hire_date "Datum zaposlenja"
FROM employees
WHERE To_Char(hire_date, 'dd') >= '11' AND To_Char(hire_date, 'dd') <= '22' AND
      To_Char(hire_date, 'mm') >= '1' AND To_Char(hire_date, 'mm') <= '2' AND
      To_Char(hire_date, 'yyyy') >= '2005' AND To_Char(hire_date, 'yyyy') <= '2010'
ORDER BY To_Char(hire_date, 'yyyy') DESC,
         To_Char(hire_date, 'mm') DESC,
         To_Char(hire_date, 'dd') DESC;

SELECT first_name || ' ' || last_name "Zaposleni",
       department_id "Sifra odjela"
FROM employees
WHERE department_id = 10 OR
      department_id = 30
ORDER BY last_name;

SELECT salary "Mjesecna plata",
       first_name "Ime zaposlenog",
       last_name "Prezime zaposlenog",
       salary * Nvl(commission_pct, 0) "Dodatak na platu"
FROM employees
WHERE salary * (1 + Nvl(commission_pct, 0)) > 10000 AND
      (department_id = 30 OR department_id = 80);

SELECT *
FROM employees
WHERE To_Char(hire_date, 'yyyy') < 2003;

SELECT first_name || ' ' || last_name "Zaposleni",
       salary * (1 + Nvl(commission_pct, 0)) "Ukupna plata",
       job_id "Posao"
FROM employees
WHERE employee_id = manager_id;

SELECT first_name || ' ' || last_name "Zaposleni",
       salary "Plata",
       salary * commission_pct "Dodatak na platu"
FROM employees
WHERE commission_pct IS NOT NULL
ORDER BY salary DESC,
         salary * commission_pct DESC;

SELECT first_name || ' ' || last_name "Zaposleni"
FROM employees
WHERE first_name || last_name LIKE '%l%l%' AND
      first_name || last_name NOT LIKE '%l%l%l%';

SELECT first_name || ' ' || last_name "Zaposleni",
       salary "Plata",
       salary * Nvl(commission_pct, 0) "Dodatak na platu"
FROM employees
WHERE salary * Nvl(commission_pct, 0) > salary * 0.2;
