1) Definisati cilj u _driver.py_ datoteci.

    ```
    bc_example.cousins(bruce, jim, 1)
    ```

2) Potražiti pravilo u datoteci _bc_example.krb_ čiji **use** dio odgovara postavljenom cilju.

    ```
    first_cousins
        use cousins(bruce, jim, 1)
        when
            child_parent(bruce, $sibling1, $_, $_)
            siblings($sibling1, $sibling2, $_, $_)
            child_parent(jim, $sibling2, $_, $_)
    ```

3) Provjeriti da li su ispunjeni uslovi pronađenog pravila, odnosno posmatrati **when** dio pravila, te konsultovati bazu činjenica _family.kfb_.

    - PRVI uslov (kompleksan):
        ```
        child_parent(bruce, $sibling1, $_, $_)
        ```

        - Pronaći među pravilima blok takav da **use** dio odgovara cilju.

            ```
            father_son
                use child_parent(bruce, $father, father, son)
                when
                    family.son_of(bruce, $father, $mother)

            mother_son
                use child_parent(bruce, $mother, mother, son)
                when
                    family.son_of(bruce, $father, $mother)

            father_daughter
                use child_parent(bruce, $father, father, daughter)
                when
                    family.daughter_of(bruce, $father, $mother)

            mother_daughter
                use child_parent(bruce, $mother, mother, daughter)
                when
                    family.daughter_of(bruce, $father, $mother)
            ```

        - Pronaći među činjenicama blok takav da se ispunjava **when** dio.

            ```
            son_of(bruce, thomas, norma)
            ```

        **Rezultat prvog uslova:**
        ```
        $sibling1 = { thomas, norma }
        ```

    - DRUGI uslov (kompleksan):
        ```
        siblings(thomas, $sibling2, $_, $_) [*]
        siblings(norma, $sibling2, $_, $_) [**]
        ```

        - Pronaći među pravilima blok takav da **use** dio odgovara cilju.

            ```
            brothers
                use siblings(___, $brother2, brother, brother)
                when
                    family.son_of(___, $father, $mother)
                    family.son_of($brother2, $father, $mother)
                    check ___ != $brother2

            sisters
                use siblings(___, $sister2, sister, sister)
                when
                    family.daughter_of(___, $father, $mother)
                    family.daughter_of($sister2, $father, $mother)
                    check ___ != $sister2

            brother_sister
                use siblings(___, $brother, brother, sister)
                when
                    family.daughter_of(___, $father, $mother)
                    family.son_of($brother, $father, $mother)

            sister_brother
                use siblings(___, $sister, sister, brother)
                when
                    family.son_of(___, $father, $mother)
                    family.daughter_of($sister, $father, $mother)
            ```

        - Pronaći među činjenicama blok takav da se ispunjava **when** dio.

            ```
            daughter_of(joyce, frederik, mary)
            daughter_of(phyllis, frederik, mary)
            son_of(thomas, frederik, mary)
            
            daughter_of(norma, allen, ismay)
            son_of(john_w, allen, ismay)
            son_of(bill, allen, ismay)
            son_of(chuck_w, allen, ismay)
            ```

        **Rezultat drugog uslova:**
        ```
        $sibling2 = { joyce, phyllis } [*]
        $sibling2 = { john_w, bill, chuck_w } [**]
        ```

    - TREĆI uslov (kompleksan):
        ```
        child_parent(jim, joyce, $_, $_)
        child_parent(jim, phyllis, $_, $_)
        child_parent(jim, john_w, $_, $_)
        child_parent(jim, bill, $_, $_)
        child_parent(jim, chuck_w, $_, $_)
        ```

        - Pronaći među pravilima blok takav da **use** dio odgovara cilju.

            ```
            father_son
                use child_parent(jim, ___, father, son)
                when
                    family.son_of(jim, ___, $mother)

            mother_son
                use child_parent(jim, ___, mother, son)
                when
                    family.son_of(jim, $father, ___)

            father_daughter
                use child_parent(jim, ___, father, daughter)
                when
                    family.daughter_of(jim, ___, $mother)

            mother_daughter
                use child_parent(jim, ___, mother, daughter)
                when
                    family.daughter_of(jim, $father, ___)
            ```

        - Pronaći među činjenicama blok takav da se ispunjava **when** dio.

            ```
            son_of(jim, bill, elvina)
            ```

        **Rezultat trećeg uslova:**
        ```
        $sibling1 = norma
        $sibling2 = bill
        ```