SELECT * FROM employees;
SELECT * FROM departments;
SELECT * FROM locations;
SELECT * FROM countries;
SELECT * FROM regions;
SELECT * FROM jobs;

-- ZADATAK 1

SELECT s.indeks,
       To_Char(t.datum, 'dd-mm-rr') datum
FROM test.studenti s, test.bodovi b, test.testovi t
WHERE b.sid = s.id AND
      b.tid = t.id AND
      Trunc(t.datum) > To_Date('01.03.201' || SubStr(Mod(b.bodovi, 13), 1, 1), 'dd.mm.yyyy');

--

SELECT SubStr(s.indeks, 5, 3) || '-' || Decode(SubStr(s.indeks, 5, 2),
                                               '5', '50%',
                                               '6', '60%',
                                               '7', '70%',
                                               '8', '80%',
                                               'Ostalo') procenat
FROM test.bodovi b, test.studenti s
WHERE b.sid = s.id AND
      LPad(Round(b.bodovi / b.sid, 0), 2, '0') > 7;

--

SELECT s.indeks
FROM test.bodovi b, test.studenti s, test.odsjeci o
WHERE b.sid = s.id AND
      o.naziv > 'L'
ORDER BY 1 DESC;

--

SELECT t.id test_id,
       Min(Months_Between(Trunc(SYSDATE, 'yy'), Trunc(t.datum, 'yyyy'))) broj_mjeseci
FROM dual d, test.testovi t
GROUP BY t.id
ORDER BY 1;

--

SELECT s.indeks,
       To_Char(t.datum, 'y') godina,
       Max(b.bodovi) bodovi
FROM test.bodovi b, test.studenti s, test.testovi t
WHERE b.tid = t.id AND
      b.sid = s.id
GROUP BY t.datum, s.indeks
ORDER BY 3 DESC;

--

SELECT To_Char(t.datum, 'dyy:yy') datum,
       Count(st.id) broj_studenata
FROM test.testovi t, test.sale sa, test.studenti st, test.bodovi b
WHERE t.said = sa.id AND
      b.tid = t.id AND
      b.sid = st.id
GROUP BY t.datum
ORDER BY 1 DESC, 2 ASC;

-- ZADATAK 3

SELECT e.first_name || ' ' || e.last_name,
       m.salary,
       e.phone_number
FROM employees e, employees m
WHERE e.manager_id = m.employee_id AND
      m.department_id <= 40
ORDER BY 2 DESC, 3 ASC;

--

SELECT last_name,
       first_name,
       hire_date,
       Round(Months_Between(SYSDATE, hire_date)),
       SYSDATE
FROM employees
ORDER BY 4;

--

SELECT d.department_name,
       Count(e.employee_id)
FROM employees e, departments d
WHERE e.department_id = d.department_id
GROUP BY d.department_name
HAVING (Count(e.employee_id) IN (3, 5) OR Count(e.employee_id) > 10)
ORDER BY 2, 1;
