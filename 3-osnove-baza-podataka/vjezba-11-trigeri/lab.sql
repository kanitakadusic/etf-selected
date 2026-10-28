SELECT * FROM employees;
SELECT * FROM departments;
SELECT * FROM locations;
SELECT * FROM countries;
SELECT * FROM regions;
SELECT * FROM jobs;

SELECT department_id, job_id, Sum(salary),
       Grouping(department_id) grp_dep, Grouping(job_id) grp_job
FROM employees
WHERE department_id < 50
GROUP BY rollup (department_id, job_id)
ORDER BY 1;

SELECT department_id, job_id, manager_id, Avg(salary)
FROM employees
WHERE department_id < 50
GROUP BY Grouping sets ((department_id, job_id), (job_id, manager_id));


SELECT ROWNUM, a.*
FROM (SELECT ROWNUM AS redni_broj, last_name, salary FROM employees ORDER BY salary DESC) a
WHERE ROWNUM <= 3;




