using LinearAlgebra

"""
- `C` matrica jediničnih cijena transporta proizvoda
- `I` vektor izvorišta sa odgovarajućom količinom proizvoda koje može poslati
- `O` vektor odredišta sa odgovarajućom količinom proizvoda koje može primiti
- `return`
    > vrijednosti početne matrice rješenja (količine koje se prenose preko odgovarajućih ruta)
    > optimalna vrijednost funkcije cilja (minimalni troškovi transporta)
"""
function nadji_pocetno_SZU(C, I, O)
    # Validacija parametara
    size(C, 1) != length(I) && throw(ArgumentError("size(C, 1) != length(I)"))
    size(C, 2) != length(O) && throw(ArgumentError("size(C, 2) != length(O)"))

    any(i -> i <= 0, I) && throw(ArgumentError("any(i -> i <= 0, I)"))
    any(o -> o <= 0, O) && throw(ArgumentError("any(o -> o <= 0, O)"))

    # Ispitivanje uvjeta balansiranosti
    sumI = sum(I)
    sumO = sum(O)

    if sumI > sumO
        push!(O, sumI - sumO)
        C = [C zeros(length(I))]
    elseif sumO > sumI
        push!(I, sumO - sumI)
        C = [C; zeros(length(O))']
    end

    # Inicijalizacija matrice količina robe koje treba transportovati
    A = zeros(length(I), length(O))

    # Metod sjeverozapadnog ugla
    i = 1
    j = 1

    while true
        i > length(I) && j > length(O) && break

        if I[i] < O[j]
            A[i, j] = I[i]
            O[j] -= I[i]
            i += 1
        elseif I[i] > O[j]
            A[i, j] = O[j]
            I[i] -= O[j]
            j += 1
        else
            A[i, j] = I[i]
            j += 1
            j <= length(O) && (A[i, j] = -1)
            i += 1
        end
    end

    mask = A .!= -1
    return A, sum((A .* C) .* mask)
end

# Udžbenik - Primjer 5.1
nadji_pocetno_SZU([
    8 9 4 6;
    6 9 5 3;
    5 6 7 4;
], [100, 120, 140], [90, 125, 80, 65]) == ([
     90  10   0   0;
      0 115   5   0;
      0   0  75  65;
], 2655)

# Udžbenik - Strana 232
nadji_pocetno_SZU([
     3 10  4  2  3;
     7  5  8  4 10;
     5  8 15  7 12;
    10 12 10  8  4;
     7  1  1  6  5;
], [200, 200, 150, 200, 250], [100, 200, 400, 200, 100]) == ([
    100 100   0   0   0;
      0 100 100   0   0;
      0   0 150   0   0;
      0   0 150  50   0;
      0   0   0 150 100;
], 8150)

# ZSR - Zadatak 1 (degeneracija)
nadji_pocetno_SZU([
    10 12  0;
     8  4  3;
     6  9  4;
     7  8  5;
], [20, 30, 20, 10], [40, 10, 30]) == ([
    20  0  0;
    20 10 -1;
     0  0 20
     0  0 10
], 530)

# ZSR - Zadatak 2
nadji_pocetno_SZU([
    40 50 80 30;
    10 60 40 50;
    80 90 20 70;
    40 60 50 20;
], [18000, 18000, 18000, 18000], [8000, 12000, 20000, 32000]) == ([
     8000 10000     0     0;
        0  2000 16000     0;
        0     0  4000 14000;
        0     0     0 18000;
], 3000000)

# ZSR - Zadatak 3
nadji_pocetno_SZU([
    4 2 5 7 6;
    7 8 3 4 5;
    2 1 4 3 2;
], [20, 110, 120], [70, 40, 30, 60, 50]) == ([
    20  0  0  0  0;
    50 40 20  0  0;
     0  0 10 60 50;
], 1130)

# ZSR - Zadatak 6 (proizvodnja > potrošnja)
nadji_pocetno_SZU([
    70 30 60;
    40 80 20;
    10 50 90;
], [750, 400, 350], [200, 450, 300]) == ([
    200 450 100   0;
      0   0 200 200;
      0   0   0 350;
], 37500)

# ZSR - Zadatak 7 (proizvodnja < potrošnja)
nadji_pocetno_SZU([
     50  10  90  40 100;
     20 100  80  70  40;
     60  80 120  60  30;
], [75, 75, 50], [60, 40, 30, 50, 40]) == ([
    60 15  0  0  0;
     0 25 30 20  0;
     0  0  0 30 20;
     0  0  0  0 20;
], 11850)