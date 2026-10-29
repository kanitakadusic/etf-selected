using LinearAlgebra

"""
- `M` težinska matrica
    * usmjeren, acikličan i topološki sortiran graf
    * prvi čvor predstavlja početni čvor
    * M[i, j] = dužina/0 za povezane/nepovezane čvorove
- `return` matrica (one-to-all)
    [indeks čvora | dužina od prvog do navedenog čvora | indeks čvora preko kojeg se dolazi u navedeni čvor]
"""
function najkraci_put(M)
    # size(M, 1) != size(M, 2) && throw(ArgumentError("Matrica nije kvadratna"))
    # any(tril(M) .!= 0) && throw(ArgumentError("M[i, j] != 0 za i >= j"))

    putevi = fill(Inf, size(M, 1), 3)
    putevi[1, :] .= [1, 0, 1]

    for i in 2:size(M, 1)
        putevi[i, 1] = i
        for j in 1:i - 1
            if M[j, i] != 0
                duzina = putevi[j, 2] + M[j, i]
                if duzina < putevi[i, 2]
                    putevi[i, 2] = duzina
                    putevi[i, 3] = j
                end
            end
        end
    end

    return putevi
end

# Testni primjer
najkraci_put([
    0  1  3  0  0  0;
    0  0  2  3  0  0;
    0  0  0 -4  9  0;
    0  0  0  0  1  2;
    0  0  0  0  0  2;
    0  0  0  0  0  0;
])

najkraci_put([
    0  1  1  0  0  0;
    0  0  2  2  8  0;
    0  0  0  2  3  0;
    0  0  0  0  2  5;
    0  0  0  0  0  1;
    0  0  0  0  0  0;
]) == [
    1  0  1;
    2  1  1;
    3  1  1;
    4  3  2;
    5  4  3;
    6  5  5;
]

najkraci_put([
    0  1  5  0  0  0;
    0  0  2  3  8  0;
    0  0  0  1  3  0;
    0  0  0  0  2  5;
    0  0  0  0  0  1;
    0  0  0  0  0  0;
]) == [
    1  0  1;
    2  1  1;
    3  3  2;
    4  4  2;
    5  6  3;
    6  7  5;
]

najkraci_put([
    0  6  0  2  0  0;
    0  0  3  3  1  0;
    0  0  0  4  0  3;
    0  0  0  0  1  0;
    0  0  0  0  0  2;
    0  0  0  0  0  0;
]) == [
    1  0  1;
    2  6  1;
    3  9  2;
    4  2  1;
    5  3  4;
    6  5  5;
]

najkraci_put([
    0  3  0  5  0  0  0;
    0  0  2  0  3  0  0;
    0  0  0  7  2  3  0;
    0  0  0  0  0  8  0;
    0  0  0  0  0  0  1;
    0  0  0  0  0  0  4;
    0  0  0  0  0  0  0;
]) == [
    1  0  1;
    2  3  1;
    3  5  2;
    4  5  1;
    5  6  2;
    6  8  3;
    7  7  5;
]

najkraci_put([
    0  2 10  7  0  0  0;
    0  0  0  3  9  0  0;
    0  0  0  6  0  6  0;
    0  0  0  0  5  9 12;
    0  0  0  0  0  1  7;
    0  0  0  0  0  0  4;
    0  0  0  0  0  0  0;
]) == [
    1  0  1;
    2  2  1;
    3 10  1;
    4  5  2;
    5 10  4;
    6 11  5;
    7 15  6;
]