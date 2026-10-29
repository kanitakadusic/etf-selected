using LinearAlgebra

# UPUTA
# ST = [izvorne | dopunske | vještačke uz ograničenja tipa nejednakosti | vještačke uz ograničenja tipa jednakosti]

"""
- `goal`        string koji označava max/min
- `c`           vektor koeficijenata u funkciji cilja
- `A`           matrica koeficijenata uz promjenjive u ograničenjima
- `b`           vektor slobodnih koeficijenata sa desne strane ograničenja
- `csigns`      vektor s elementima +1(>=), -1(<=), 0(==) koji odgovaraju znaku ograničenja
- `vsigns`      vektor s elementima +1(nenegativna), -1(nepozitivna), 0(neograničena po znaku) koji odgovara ograničenjima uz promjenjive
- `return`
        - Z     optimalna vrijednost funkcije cilja
        - X     vrijednosti izvornih primalnih promjenjivih
        - Xd    vrijednosti dopunskih primalnih promjenjivih
        - Y     vrijednosti izvornih dualnih promjenjivih
        - Yd    vrijednosti dopunskih dualnih promjenjivih
        - status
                0: Postupak je okončan normalno i nađeno je jedinstveno nedegenerirano optimalno rješenje.
                1: Postupak je okončan normalno i nađeno je jedinstveno degenerirano optimalno rješenje.
                2: Postupak je okončan normalno i nađeno je jedno optimalno rješenje, ali koje nije jedinstveno.
                3: Funkcija cilja nije ograničena na dopustivoj oblasti, odnosno optimalno rješenje se nalazi u beskonačnosti.
                4: Dopustiva oblast ne postoji, odnosno problem nema rješenja.
                5: Greška u zadavanju parametara, odnosno zadani parametri nisu smisleni.
"""                
function general_simplex(goal, c, A, b, csigns=nothing, vsigns=nothing, eps=1e-10)
    m, n = size(A)

    if csigns == nothing
        if goal == "max"
            csigns = -ones(m)
        elseif goal == "min"
            csigns = ones(m)
        end
    end

    if vsigns == nothing
        vsigns = ones(n)
    end

    # Provjera ulaznih parametara
    if (
        goal != "max" && goal != "min" ||
        any(sign -> sign != 1 && sign != -1 && sign != 0, csigns) ||
        any(sign -> sign != 1 && sign != -1 && sign != 0, vsigns) ||

        length(b) != m ||
        length(c) != n ||

        length(csigns) != m ||
        length(vsigns) != n
    )
        return NaN, nothing, nothing, nothing, nothing, nothing, 5
    end

    # Rukovanje negativnim slobodnim koeficijentima
    constraintMultipliedBy = ones(Int, m)
    for i in 1:m
        if b[i] < 0
            constraintMultipliedBy[i] = -1
            A[i, :] .*= -1
            csigns[i] *= -1
            b[i] *= -1
        end
    end

    # Za potrebe algoritma...
    numOfSlack = count(sign -> sign == -1 || sign == 1, csigns)
    numOfArtificial_1 = count(sign -> sign == 1, csigns)
    numOfArtificial_0 = count(sign -> sign == 0, csigns)

    # Određivanje spregnutosti dualnih izvornih promjenjivih
    counterSlack = 0
    counterArtificial_0 = 0

    dualInitials = []
    for i in 1:m
        if csigns[i] == -1 # c: <=
            counterSlack += 1
            push!(dualInitials, (n + counterSlack) * (goal == "max" ? -1 : 1) * constraintMultipliedBy[i])
        elseif csigns[i] == 1 # c: >=
            counterSlack += 1
            push!(dualInitials, (n + counterSlack) * (goal == "max" ? 1 : -1) * constraintMultipliedBy[i])
        else # c: ==
            counterArtificial_0 += 1
            push!(dualInitials, (n + numOfSlack + numOfArtificial_1 + counterArtificial_0) * (goal == "max" ? -1 : 1) * constraintMultipliedBy[i])
        end
    end

    # Pretvaranje problema minimizacije u problem maksimizacije
    goal == "min" && (c .*= -1)

    # Određivanje spregnutosti dualnih dopunskih promjenjivih
    dualSlacks = []
    for i in 1:n
        if vsigns[i] == 1 # x >= 0
            push!(dualSlacks, -i)
        elseif vsigns[i] == -1 # x <= 0
            push!(dualSlacks, i)
        end
    end

    # Rukovanje atipičnim znakovima promjenjivih
    j = 1
    for sign in vsigns
        if sign == -1 # x <= 0
            c[j] *= -1
            A[:, j] .*= -1
        elseif sign == 0 # x n.p.z.
            insert!(c, j + 1, -c[j])
            A = hcat(A[:, 1:j], -A[:, j], A[:, j + 1:end])
            n += 1
            j += 1
        end
        j += 1
    end

    # Inicijalizacija simplex tabele
    numOfInitial = n

    matrixWithSlack = zeros(m + 2, 0)
    matrixWithArtificial_1 = zeros(m + 2, 0)
    matrixWithArtificial_0 = zeros(m + 2, 0)

    M_rowBelowInitialVariables = zeros(n)'
    M_value = 0

    B = Int[]

    for i in 1:m
        if csigns[i] == -1 # c: <=
            col = zeros(m + 2)
            col[i] = 1
            matrixWithSlack = [matrixWithSlack col]

            n += 1
            push!(B, numOfInitial + size(matrixWithSlack, 2))
        elseif csigns[i] == 1 # c: >=
            col = zeros(m + 2)
            col[i] = col[m + 1] = -1
            matrixWithSlack = [matrixWithSlack col]

            col = zeros(m + 2)
            col[i] = 1
            matrixWithArtificial_1 = [matrixWithArtificial_1 col]

            M_rowBelowInitialVariables[:] .+= A[i, :]
            M_value += b[i]

            n += 2
            push!(B, numOfInitial + numOfSlack + size(matrixWithArtificial_1, 2))
        else # c: ==
            col = zeros(m + 2)
            col[i] = 1
            matrixWithArtificial_0 = [matrixWithArtificial_0 col]

            M_rowBelowInitialVariables[:] .+= A[i, :]
            M_value += b[i]

            n += 1
            push!(B, numOfInitial + numOfSlack + numOfArtificial_1 + size(matrixWithArtificial_0, 2))
        end
    end

    A = [A; M_rowBelowInitialVariables; c']
    b = [b; M_value; 0]
    ST = [A matrixWithSlack matrixWithArtificial_1 matrixWithArtificial_0 b]

    isDegenerate = false

    # Transformacija simplex tabele
    while true
        # Pronalaženje promjenjive koja ulazi u bazu
        atLeastOneArtificialInBase = any(v -> v > numOfInitial + numOfSlack, B)
        row = atLeastOneArtificialInBase ? m + 1 : m + 2
        cMax, q = findmax(ST[row, 1:numOfInitial + numOfSlack])

        # Ako ni jedna promjenjiva nije kandidat za bazu...
        if cMax <= 0
            # ...nema rješenja! :(
            atLeastOneArtificialInBase && return NaN, nothing, nothing, nothing, nothing, nothing, 4
            
            # ...optimalno rješenje je nađeno! :D
            break
        end

        # Pronalaženje promjenjive koja napušta bazu
        tMax = Inf
        p = nothing

        for i in 1:m
            if ST[i, q] > 0
                t = ST[i, end] / ST[i, q]
                
                # Uzima se u obzir problem degeneracije
                if t < tMax || abs(t - tMax) < eps && (isDegenerate = true) && rand() >= 0.5
                    tMax = t
                    p = i
                end
            end
        end

        # Optimalno rješenje je neograničeno
        tMax == Inf && return Inf, nothing, nothing, nothing, nothing, nothing, 3

        # Promjena baze
        B[p] = q

        # Transformacija
        pivot = ST[p, q]
        ST[p, :] ./= pivot

        for i in 1:m + 2
            if i != p
                factor = ST[i, q]
                ST[i, :] .-= factor * ST[p, :]
            end
        end

        # Zaokruživanje vrlo malih vrijednosti na nulu
        ST .= ifelse.(abs.(ST) .< eps, 0, ST)
    end

    lastRow = ST[end, :]

    # Očitavanje optimalnog rješenja iz tabele
    X = zeros(numOfInitial + numOfSlack)
    X[B[:]] .= ST[1:m, end]

    # Vraćanje smjena
    for j in 1:length(vsigns)
        if vsigns[j] == -1 # x <= 0
            X[j] *= -1
            lastRow[j] *= -1
        elseif vsigns[j] == 0 # x n.p.z.
            X[j] -= X[j + 1]
            deleteat!(X, j + 1)
            lastRow[j] -= lastRow[j + 1]
            deleteat!(lastRow, j + 1)
        end
    end

    # Ispitivanje prirode rješenja
    status = 0

    # Nejedinstveno optimalno rješenje
    count(e -> e == 0, lastRow[1:length(vsigns) + numOfSlack]) > m && (status = 2)
    # Jedinstveno degenerirano optimalno rješenje
    isDegenerate && (status = 1)

    # Jedinstveno nedegenerirano optimalno rješenje
    Xd = X[length(vsigns) + 1:end]
    csigns = Float64.(csigns)
    index = 1
    for i in 1:length(csigns)
        if csigns[i] == 1 || csigns[i] == -1
            csigns[i] = Xd[index]
            index += 1
        end
    end

    Yd = [lastRow[abs(xn)] * sign(xn) for xn in dualSlacks]
    vsigns = Float64.(vsigns)
    index = 1
    for j in 1:length(vsigns)
        if vsigns[j] == 1 || vsigns[j] == -1
            vsigns[j] = Yd[index]
            index += 1
        end
    end

    return (
        goal == "max" ? -ST[end, end] : ST[end, end],
        X[1:length(vsigns)],
        csigns,
        [lastRow[abs(xn)] * sign(xn) for xn in dualInitials],
        vsigns,
        status
    )
end

function runTest(X, Y, eps = 1e-7)
    for (x, y) in zip(X, Y)
        if isa(x, AbstractVector)
            !all(map((a, b) -> abs(a - b) < eps, x, y)) && throw(":(")
        elseif x == nothing || x == Inf || isnan(x)
            continue
        else
            !(abs(x - y) < eps) && throw(":(")
        end
    end
    return ":)"
end

begin
    # `testni slučaj`   Standardni problem maksimizacije. Sva ograničenja su uska grla.
    # `izvor`           ZSR - Zadatak 8
    runTest(general_simplex(
        "max",
        [4, 4.7, 3.5],
        [0.125 0.15 0.1; 0.5 0.5 0.5],
        [40, 150]
    ), (1290, [0, 200, 100], [0, 0], [24, 2.2], [0.1, 0, 0], 0))

    # `testni slučaj`   Standardni problem minimizacije. Sva ograničenja su uska grla.
    # `izvor`           ZSR - Zadatak 15
    runTest(general_simplex(
        "min",
        [24, 25],
        [1 5; 4 1],
        [17, 11]
    ), (123, [2, 3], [0, 0], [4, 5], [0, 0], 0))

    # `testni slučaj`   Standardni problem maksimizacije. Postoje ograničenja koja nisu uska grla.
    # `izvor`           ZSR - Zadatak 9
    runTest(general_simplex(
        "max",
        [8, 16, 29],
        [1 3 5; 1 2 3; 3 8 14],
        [40, 30, 100]
    ), (250, [24, 0, 2], [6, 0, 0], [0, 5, 1], [0, 2, 0], 0))

    # `testni slučaj`   Standardni problem minimizacije. Postoje ograničenja koja nisu uska grla.
    # `izvor`           ZSR - Zadatak 16
    runTest(general_simplex(
        "min",
        [5, 2],
        [15 20; 15 10],
        [100, 70]
    ), (14, [0, 7], [40, 0], [0, 0.2], [2, 0], 0))

    # `testni slučaj`   Standardni problem maksimizacije. Optimalno rješenje nije jedinstveno.
    # `izvor`           ZSR - Zadatak 6
    runTest(general_simplex(
        "max",
        [8, 10, 12],
        [1 1 1; 1 2 3],
        [8, 12]
    ), (72, [6, 0, 2], [0, 0], [6, 2], [0, 0, 0], 2))

    # `testni slučaj`   Standardni problem minimizacije. Optimalno rješenje nije jedinstveno.
    # `izvor`           ZSR - Zadatak 17
    runTest(general_simplex(
        "min",
        [4, 6],
        [20 15; 10 15],
        [105, 75]
    ), (30, [3, 3], [0, 0], [0, 0.4], [0, 0], 2))

    # `testni slučaj`   Maksimizacija. Različite vrste ograničenja.
    # `izvor`           ZSR - Zadatak 31
    runTest(general_simplex(
        "max",
        [20, 10, 5],
        [1 1 1; 2 1 1; 2 1.5 0.5],
        [100, 120, 110],
        [0, 1, -1],
        [1, 1, 1]
    ), (1100, [40, 0, 60], [0, 20, 0], [0, 0, 10], [0, 5, 0], 0))

    # `testni slučaj`   Minimizacija. Različite vrste ograničenja.
    # `izvor`           ZSR - Zadatak 21
    runTest(general_simplex(
        "min",
        [0.4, 0.5],
        [0.3 0.1; 0.5 0.5; 0.6 0.4],
        [2.7, 6, 6],
        [-1, 0, 1],
        [1, 1]
    ), (5.25, [7.5, 4.5], [0, 0, 0.3], [-0.5, 1.1, 0], [0, 0], 0))

    # `testni slučaj`   Minimizacija. Različite vrste znakova promjenjivih.
    # `izvor`           Ispit - Zadatak 4 (Primal)[*]
    runTest(general_simplex(
        "min",
        [8, -10, 15],
        [-2 0 -3; 3 -5 2],
        [-3, 5],
        [-1, 0],
        [1, -1, 1]
    ), (13, [1.5, -0.1, 0], [0, 0], [-1, 2], [0, 0, 8], 0))

    # `testni slučaj`   Maksimizacija. Različite vrste znakova promjenjivih.
    # `izvor`           Ispit - Zadatak 4 (Dual)[*]
    runTest(general_simplex(
        "max",
        [-3, 5],
        [-2 3; 0 -5; -3 2],
        [8, -10, 15],
        [-1, 1, -1],
        [-1, 0]
    ), (13, [-1, 2], [0, 0, 8], [1.5, -0.1, 0], [0, 0], 0))

    # `testni slučaj`   Minimizacija. Različite vrste znakova promjenjivih.
    # `izvor`           Udžbenik - Primjer 4.3
    runTest(general_simplex(
        "min",
        [6, 18, 24],
        [1 0 3; 0 2 2],
        [5, 2],
        [1, 1],
        [0, 1, -1]
    ), (48, [5, 1, 0], [0, 0], [6, 9], [0, 0, 12], 0))

    # `testni slučaj`   Optimalno rješenje je neograničeno.
    # `izvor`           Udžbenik - Primjer 3.11
    runTest(general_simplex(
        "max",
        [1, 2],
        [-2 1; 0 1],
        [1, 3],
        [-1, -1],
        [1, 1]
    ), (Inf, nothing, nothing, nothing, nothing, nothing, 3))

    # `testni slučaj`   Nema rješenja.
    # `izvor`           Primjer konstruisan u cilju postizanja kontradiktornih ograničenja.
    runTest(general_simplex(
        "max",
        [1, 2],
        [-2 1; 0 1],
        [1, 3],
        [-1, 1],
        [-1, -1]
    ), (NaN, nothing, nothing, nothing, nothing, nothing, 4))

    # `testni slučaj`   Nema rješenja.
    # `izvor`           Primjer konstruisan u cilju postizanja kontradiktornih ograničenja.
    runTest(general_simplex(
        "min",
        [1, 2, 3],
        [-2 -1 -5; 0 1 2],
        [1, 3],
        [1, 1],
        [1, 1, 1]
    ), (NaN, nothing, nothing, nothing, nothing, nothing, 4))

    # `testni slučaj`   Problem degeneracije.
    # `izvor`           Udžbenik - Strana 121
    runTest(general_simplex(
        "min",
        [-3, 80, -2, 24],
        [0.25 -8 -1 9; 0.5 -12 -0.5 3; 0 0 1 0],
        [0, 0, 1],
        [-1, -1, -1],
        [1, 1, 1, 1]
    ), (-5, [1, 0, 1, 0], [0.75, 0, 0], [0, -6, -5], [0, 8, 0, 42], 1))

    # `testni slučaj`   Problem degeneracije.
    # `izvor`           Internet
    runTest(general_simplex(
        "max",
        [10, -57, -9, -24],
        [0.5 -5.5 -2.5 9; 0.5 -1.5 -0.5 1; 1 0 0 0],
        [0, 0, 1],
        [-1, -1, -1],
        [1, 1, 1, 1]
    ), (1, [1, 0, 1, 0], [2, 0, 0], [0, 18, 1], [0, 30, 0, 42], 1))

    # Još neki primjeri...

    # `testni slučaj`   Standardni problem maksimizacije.
    # `izvor`           Udžbenik - Primjer 3.8
    runTest(general_simplex(
        "max",
        [3, 1],
        [0.5 0.3; 0.1 0.2],
        [150, 60],
        [-1, -1],
        [1, 1]
    ), (900, [300, 0], [0, 30], [6, 0], [0, 0.8], 0))

    # `testni slučaj`   Standardni problem maksimizacije.
    # `izvor`           Udžbenik - Primjer 3.10
    runTest(general_simplex(
        "max",
        [800, 1000],
        [30 16; 14 19; 11 26; 0 1],
        [22800, 14100, 15950, 550],
        [-1, -1, -1, -1],
        [1, 1]
    ), (780000, [600, 300], [0, 0, 1550, 250], [600/173, 8600/173, 0, 0], [0, 0], 0))

    # `testni slučaj`   Standardni problem minimizacije.
    # `izvor`           Udžbenik - Primjer 3.13 (Primal)[**]
    runTest(general_simplex(
        "min",
        [40, 30],
        [0.1 0; 0 0.1; 0.5 0.3; 0.1 0.2],
        [0.2, 0.3, 3, 1.2],
        [1, 1, 1, 1],
        [1, 1]
    ), (1860/7, [24/7, 30/7], [1/7, 9/70, 0, 0], [0, 0, 500/7, 300/7], [0, 0], 0))

    # `testni slučaj`   Standardni problem maksimizacije.
    # `izvor`           Udžbenik - Primjer 4.5 (Dual)[**]
    runTest(general_simplex(
        "max",
        [0.2, 0.3, 3, 1.2],
        [0.1 0 0.5 0.1; 0 0.1 0.3 0.2],
        [40, 30],
        [-1, -1],
        [1, 1, 1, 1]
    ), (1860/7, [0, 0, 500/7, 300/7], [0, 0], [24/7, 30/7], [1/7, 9/70, 0, 0], 0))

    # `testni slučaj`   Maksimizacija. Sva ograničenja tipa jednakosti.
    # `izvor`           Udžbenik - Primjer 4.9 (Primal)[***]
    runTest(general_simplex(
        "max",
        [100, 300, 200],
        [1 2 1; 3 1 2],
        [4, 9],
        [0, 0],
        [1, 1, 1]
    ), (700, [1, 0, 3], [0, 0], [400, -100], [0, 400, 0], 0))

    # `testni slučaj`   Minimizacija. Sve promjenjive neograničene po znaku.
    # `izvor`           Udžbenik - Primjer 4.9 (Dual)[***]
    runTest(general_simplex(
        "min",
        [4, 9],
        [1 3; 2 1; 1 2],
        [100, 300, 200],
        [1, 1, 1],
        [0, 0]
    ), (700, [400, -100], [0, 400, 0], [1, 0, 3], [0, 0], 0))

    # `testni slučaj`   Maksimizacija.
    # `izvor`           Udžbenik - Primjer 4.11
    runTest(general_simplex(
        "max",
        [40, 30],
        [3 2; 1 1; 2 1; 3 4; 1 0; 0 1],
        [300, 80, 200, 360, 60, 60],
        [-1, -1, -1, -1, -1, -1],
        [1, 1]
    ), (3000, [60, 20], [80, 0, 60, 100, 0, 40], [0, 30, 0, 0, 10, 0], [0, 0], 0))

    # Priloženi testni primjeri...

    # Test 1
    runTest(general_simplex(
        "max",
        [40, 30],
        [3 1.5; 1 1; 2 1; 3 4; 1 0; 0 1],
        [300, 80, 200, 360, 60, 60],
        [-1, -1, -1, -1, -1, -1],
        [1, 1]
    ), (3000, [60, 20], [90, 0, 60, 100, 0, 40], [0, 30, 0, 0, 10, 0], [0, 0], 0))

    # Test 2
    runTest(general_simplex(
        "min",
        [1, 1.5],
        [2 1; 1 1; 1 1],
        [10, 8, 12],
        [1, 1, 1],
        [1, 1]
    ), (12, [12, 0], [14, 4, 0], [0, 0, 1], [0, 0.5], 0))

    # Test 3
    runTest(general_simplex(
        "min",
        [32, 56, 50, 60],
        [1 1 1 1; 250 150 400 200; 0 0 0 1; 0 1 1 0],
        [1, 300, 0.3, 0.5],
        [0, 1, -1, -1],
        [1, 1, 1, 1]
    ), (38, [2/3, 0, 1/3, 0], [0, 0, 0.3, 1/6], [2, 0.12, 0, 0], [0, 36, 0, 34], 0))

    # Test 4
    runTest(general_simplex(
        "max",
        [1, 300, -0.3, -0.5],
        [1 250 0 0; 1 150 0 -1; 1 400 0 -1; 1 200 -1 0],
        [32, 56, 50, 60],
        [-1, -1, -1, -1],
        [0, 1, 1, 1]
    ), (38, [2, 0.12, 0, 0], [0, 36, 0, 34], [2/3, 0, 1/3, 0], [0, 0, 0.3, 1/6], 0))

    # Test 5
    runTest(general_simplex(
        "max",
        [1, 1],
        [-2 1; -1 2],
        [-1, 4],
        [-1, 1],
        [1, 1]
    ), (Inf, nothing, nothing, nothing, nothing, nothing, 3))

    # Test 6
    runTest(general_simplex(
        "max",
        [1, 2],
        [1 1; 3 3],
        [2, 4],
        [1, -1],
        [1, 1]
    ), (NaN, nothing, nothing, nothing, nothing, nothing, 4))

    # Test 7
    runTest(general_simplex(
        "max",
        [4000, 2000],
        [3 3; 2 1; 1 0; 0 1],
        [12000, 6000, 2500, 3000],
        [-1, -1, -1, -1],
        [1, 1]
    ), (12e6, [2500, 1000], [1500, 0, 0, 2000], [0, 2000, 0, 0], [0, 0], 2))

    # Test 8
    runTest(general_simplex(
        "max",
        [3, 9],
        [1 4; 1 2],
        [8, 4],
        [-1, -1],
        [1, 1]
    ), (18, [0, 2], [0, 0], [1.5, 1.5], [0, 0], 1))
end