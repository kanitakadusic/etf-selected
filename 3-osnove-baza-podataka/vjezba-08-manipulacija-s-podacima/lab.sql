SELECT * FROM employees;
SELECT * FROM departments;
SELECT * FROM locations;
SELECT * FROM countries;
SELECT * FROM regions;
SELECT * FROM jobs;

SELECT * FROM z19327 ORDER BY employee_id DESC;
SELECT * FROM o19327 ORDER BY department_id DESC;

-- ZADATAK 1

CREATE TABLE z19327 AS SELECT * FROM employees;

-- ZADATAK 2

DESC TABLE employees; -- ne radi

-- ZADATAK 3

INSERT INTO z19327
VALUES (210, 'Red', 'Apple', 'RAPPLE', '111.111.1111', SYSDATE, 'IT_PROG', 15000, NULL, 100, 20);

INSERT INTO z19327
VALUES (211, 'Yellow', 'Banana', 'YBANANA', '222.222.2222', SYSDATE, 'IT_PROG', 16000, NULL, 100, 20);

-- ZADATAK 4

UPDATE z19327
SET commission_pct = 0.13
WHERE salary < 3000;

-- ZADATAK 5

UPDATE z19327 z
SET (salary, commission_pct) = (SELECT Decode(e.commission_pct, NULL, e.salary * 0.9, e.salary * (1 + e.commission_pct)),
                                       Decode(e.commission_pct, NULL, 0.15, NULL)
                                FROM z19327 e
                                WHERE e.employee_id = z.employee_id)
WHERE employee_id IN (SELECT e.employee_id, e.salary, e.commission_pct
                      FROM z19327 e, departments d, locations l
                      WHERE e.department_id = d.department_id AND
                            d.location_id = l.location_id AND
                            Lower(l.city) = 'toronto');

-- ZADATAK 6

UPDATE z19327
SET department_id = (SELECT department_id FROM departments WHERE Lower(department_name) = 'marketing')
WHERE employee_id IN (SELECT e.employee_id
                      FROM z19327 e, departments d, locations l, countries c
                      WHERE e.department_id = d.department_id AND
                            d.location_id = l.location_id AND
                            l.country_id = c.country_id AND
                            Lower(c.country_name) LIKE '%america%' AND
                            e.salary < (SELECT Avg(salary)
                                        FROM employees
                                        WHERE department_id = e.department_id AND
                                              employee_id <> e.employee_id) AND
                            e.salary NOT IN (SELECT Min(salary) FROM employees GROUP BY department_id) AND
                            e.salary NOT IN (SELECT Max(salary) FROM employees GROUP BY department_id));

-- ZADATAK 7

--

-- ZADATAK 8

CREATE TABLE o19327 AS SELECT * FROM departments;

-- ZADATAK 9

UPDATE o19327 o
SET o.department_name = (SELECT Decode(Lower(c.country_name), 'united states of america', 'US ->> ', 'OS ->> ') || department_name
                         FROM o19327 d, locations l, countries c
                         WHERE d.department_id = o.department_id AND
                               d.location_id = l.location_id AND
                               l.country_id = c.country_id);

-- ZADATAK 10

DELETE FROM z19327
WHERE employee_id IN (SELECT e.employee_id
                      FROM z19327 e, departments d
                      WHERE e.department_id = d.department_id AND
                            Lower(d.department_name) LIKE '%a%');

-- ZADATAK 11

DELETE FROM o19327
WHERE department_id IN (SELECT d.department_id
                        FROM z19327 e, o19327 d
                        WHERE e.department_id(+) = d.department_id
                        GROUP BY d.department_id
                        HAVING Count(e.employee_id) = 0);

-- ZADATAK 12

--
