using LinearAlgebra

"""
- `M::Matrix{Float64}`
- `return`
    1) slučaj: OPTIMALNO RJEŠENJE JESTE NAĐENO [k == n]
        > arrangement::Matrix{Float64}
            *) 1, ako izvršilac I(i) JESTE raspoređen na mjesto M(j)
            *) 0, ako izvršilac I(i) NIJE raspoređen na mjesto M(j)
        > optimum::Float64
    2) slučaj: OPTIMALNO RJEŠENJE NIJE NAĐENO [k <= n]
        > arrangement::Matrix{Float64}
            *) +0, za NEZAVISNE nule
            *) -0, za ZAVISNE nule
            *) ostale vrijednosti odgovaraju početnoj matrici nakon redukcija
        > optimum::Float64
"""
function rasporedi(M::Matrix{Float64})
    m, n = size(M)
    d = maximum(size(M))
    
    # Balansiranje
    if m > n
        # Dodavanje fiktivnih mjesta
        M = hcat(M, zeros(m, m - n))
    elseif m < n
        # Dodavanje fiktivnih izvršilaca
        M = vcat(M, zeros(n - m, n))
    end

    # Kopija cijena prije transformacija
    PRICES = copy(M)

    # Redukcija po redovima
    M .= mapslices(row -> row .- minimum(row), M, dims=2)

    # Redukcija po kolonama
    M .= mapslices(col -> col .- minimum(col), M, dims=1)

    # Razvrstavanje nula
    unclassifiedZerosCounter = 0
    indepententZerosCounter = 0

    for i in 1:d
        for j in 1:d
            if M[i, j] == 0.0
                M[i, j] = NaN
                unclassifiedZerosCounter += 1
            end
        end
    end

    ARRANGEMENT = zeros(d, d)
    Z = 0

    while true
        for i in 1:d
            indices = findall(c -> isnan(c), M[i, :])
    
            if length(indices) == 1
                M[i, indices[1]] = +0.0 # nezavisna
                ARRANGEMENT[i, indices[1]] = 1
                Z += PRICES[i, indices[1]]

                unclassifiedZerosCounter -= 1
                indepententZerosCounter += 1
    
                for k in 1:d
                    if isnan(M[k, indices[1]])
                        M[k, indices[1]] = -0.0 # zavisna
                        unclassifiedZerosCounter -= 1
                    end
                end
            end
        end

        # Provjeriti da li su sve nule razvrstane
        unclassifiedZerosCounter <= 0 && break
    
        for j in 1:d
            indices = findall(c -> isnan(c), M[:, j])
    
            if length(indices) == 1
                M[indices[1], j] = +0.0 # nezavisna
                ARRANGEMENT[indices[1], j] = 1
                Z += PRICES[indices[1], j]

                unclassifiedZerosCounter -= 1
                indepententZerosCounter += 1
    
                for k in 1:d
                    if isnan(M[indices[1], k])
                        M[indices[1], k] = -0.0 # zavisna
                        unclassifiedZerosCounter -= 1
                    end
                end
            end
        end

        # Provjeriti da li su sve nule razvrstane
        unclassifiedZerosCounter <= 0 && break
    end

    # Ukoliko nikada nije bilo dileme koju nulu proglasiti za nezavisnu,
    # pronađeni skup nezavisnih nula je garantirano maksimalan.
    # Uzimajući u obzir postavku, pronađen je maksimalan skup nezavisnih nula.

    # Ukoliko je veličina maksimalnog skupa nezavisnih nula jednaka dimenziji matrice,
    # optimalno rješenje je nađeno.
    # U suprotnom bi trebalo nastaviti s postupkom.

    return indepententZerosCounter == d ? ARRANGEMENT : M, Z
end

# Testni primjer 1 [k == n]
rasporedi(Float64[
    80 20 23;
    31 40 12;
    61  1  1;
]) == ([
    0 1 0;
    1 0 0;
    0 0 1;
], 52.0)

# Testni primjer 2 [k == n]
rasporedi(Float64[
     25  55  40  80;
     75  40  60  95;
     35  50 120  80;
     15  30  55  65;
]) == ([
    0 0 1 0;
    0 1 0 0;
    0 0 0 1;
    1 0 0 0;
], 175.0)

# Udžbenik - Primjer 6.2 (I dio: redukcije + razvrstavanje) [k <= n]
rasporedi(Float64[
    11 17  8 16 20;
     9  7 12  6 15;
    13 16 15 12 16;
    21 24 17 28 26;
    14 10 12 11 15;
]) == ([
      2.0  9.0   0.0   8.0   8.0;
      2.0  1.0   6.0   0.0   5.0;
      0.0  4.0   3.0  -0.0  -0.0;
      3.0  7.0  -0.0  11.0   5.0;
      3.0  0.0   2.0   1.0   1.0;
], 37.0)

# Udžbenik - Primjer 6.2 (II dio: razvrstavanje) [k == n]
rasporedi(Float64[
    0 7 0 6 6;
    2 1 8 0 5;
    0 4 5 0 0;
    1 5 0 9 3;
    3 0 4 1 1;
]) == ([
    1 0 0 0 0;
    0 0 0 1 0;
    0 0 0 0 1;
    0 0 1 0 0;
    0 1 0 0 0;
], 0.0)

# Udžbenik - Primjer 6.3 (I dio: redukcije + razvrstavanje) [k <= n]
rasporedi(Float64[
     -5  -6  -5  -1;
     -4  -6  -4  -1;
     -8  -6  -7  -6;
     -2  -4  -4  -4;
     -6 -10  -9  -4;
]) == ([
     1.0  0.0  1.0  5.0  2.0;
     2.0 -0.0  2.0  5.0  2.0;
     0.0  2.0  1.0  2.0  4.0;
     2.0 -0.0  0.0 -0.0 -0.0;
     4.0 -0.0  1.0  6.0  6.0;
], -18.0)

# Udžbenik - Primjer 6.3 (II dio: razvrstavanje) [k <= n]
rasporedi(Float64[
    1 2 2 3 0;
    2 2 3 3 0;
    0 4 2 0 2;
    4 4 3 0 0;
    2 0 0 2 2;
]) == ([
     1.0  2.0  2.0  3.0  0.0;
     2.0  2.0  3.0  3.0 -0.0;
     0.0  4.0  2.0 -0.0  2.0;
     4.0  4.0  3.0  0.0 -0.0;
     2.0  0.0 -0.0  2.0  2.0;
], 0.0)

# Udžbenik - Primjer 6.3 (III dio: razvrstavanje) [k <= n]
rasporedi(Float64[
    0 1 1 2 0;
    1 1 2 2 0;
    0 4 2 0 3;
    4 4 3 0 1;
    2 0 0 2 3;
]) == ([
     0.0  1.0  1.0  2.0 -0.0;
     1.0  1.0  2.0  2.0  0.0;
    -0.0  4.0  2.0 -0.0  3.0;
     4.0  4.0  3.0  0.0  1.0;
     2.0  0.0 -0.0  2.0  3.0;
], 0.0)