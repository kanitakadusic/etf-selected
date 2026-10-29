using LinearAlgebra

function networkGraph(A, P, T)
    # Matrica aktivnosti + fiktivni početak i fiktivni kraj
    network = fill(-1, length(A) + 2, length(A) + 2)

    # Početna aktivnost
    for i in 1:length(A)
        if P[i] == "-"
            network[1, i + 1] = 0
        end
    end

    # Aktivnosti
    for i in 1:length(A), j in 1:length(A)
        if occursin(A[i], P[j])
            network[i + 1, j + 1] = T[i]
        end
    end
    
    # Krajnja aktivnost
    for (i, activity) in enumerate(A)
        if !any(predecessors -> occursin(activity, predecessors), P)
            network[i + 1, end] = T[i]
        end
    end

    return network
end

function topologicalOrder(M, nonexistentBranchValue = 0)
    # Kreiranje matrice adjacencije
    adjacencyMatrix = (M .!= nonexistentBranchValue) .|> Int

    # Lista topološki sortiranih čvorova
    topologicalOrder = Int[]

    while length(topologicalOrder) < size(adjacencyMatrix, 1)
        # Pronalazak prvog čvora u koji ne ulazi niti jedna grana
        nodeIndex = findfirst(column -> sum(adjacencyMatrix[:, column]) == 0, 1:size(adjacencyMatrix, 2))

        # Brisanje grana koje izlaze iz pronađenog čvora
        adjacencyMatrix[nodeIndex, adjacencyMatrix[nodeIndex, :] .== 1] .= 0

        # Pronađeni čvor je sljedeći u nizu topološki sortiranih čvorova
        push!(topologicalOrder, nodeIndex)

        # Osigurati da se pronađeni čvor ponovo ne pojavi
        adjacencyMatrix[nodeIndex, nodeIndex] = 10
    end

    return topologicalOrder
end

"""
 - `A::AbstractVector`   aktivnosti
 - `P::AbstractVector`   prethodnici
 - `T::AbstractVector`   trajanja aktivnosti
 - `return`
      > Z::Float         trajanje
      > put::String      kritični put
"""
function cpm(A::AbstractVector, P::AbstractVector, T::AbstractVector)
    if (
        length(A) != length(P) ||
        length(A) != length(T) ||

        length(A) != length(unique(A)) ||
        !any(p -> p == "-", P) ||
        any(t -> t <= 0, T)
    )
        return nothing
    end

    # Analiza strukture
    structure = nothing
    begin
        graph = networkGraph(A, P, T)
        order = topologicalOrder(graph, -1)
        structure = graph[order, order]

        popfirst!(order)
        pop!(order)
        order .-= 1

        A = A[order]
        P = P[order]
        T = T[order]
    end

    # Analiza vremena
    time = fill(-1., size(structure, 1), 4)
    time[:, 1] .= -Inf
    time[:, 3] .= Inf

    time[1, 1] = 0
    # Bellman unaprijed
    for i in 2:size(structure, 1)
        for j in 1:i - 1
            if structure[j, i] != -1
                duzina = time[j, 1] + structure[j, i]
                if duzina > time[i, 1]
                    time[i, 1] = duzina
                    time[i, 2] = j
                end
            end
        end
    end

    time[end, 3] = time[end, 1]
    # Bellman unazad
    for i in size(structure, 1) - 1:-1:1
        for j in i + 1:size(structure, 2)
            if structure[i, j] != -1
                duzina = time[j, 3] - structure[i, j]
                if duzina < time[i, 3]
                    time[i, 3] = duzina
                    time[i, 4] = j
                end
            end
        end
    end

    # Određivanje kritičnih puteva
    criticalPaths = []
    # Prolazak kroz sve aktivnosti
    for i in 1:length(A)
        # Provjera da li je aktivnost kritična
        if time[i + 1, 1] == time[i + 1, 3]
            # Ako je kritična aktivnost početna kritična aktivnost...
            if P[i] == "-"
                # ... započeti zapis novog kritičnog puta
                push!(criticalPaths, [A[i]])
            else
                numberOfCriticalPaths = length(criticalPaths)
                # U suprotnom proći kroz sve kritične puteve
                for j in 1:numberOfCriticalPaths
                    # Ako kritičnoj aktivnosti prethodi aktivnost koja se nalazi na kraju puta...
                    if occursin(criticalPaths[j][end], P[i])
                        # ... dodati je na put
                        push!(criticalPaths[j], A[i])
                    else
                        # U suprotnom potražiti prethodnika među ranijim aktivnostima
                        for k in length(criticalPaths[j]) - 1:-1:1
                            if occursin(criticalPaths[j][k], P[i])
                                newPath = criticalPaths[j][1:k]
                                push!(newPath, A[i])
                                push!(criticalPaths, newPath)
                            end
                        end
                    end
                end
            end
        end
    end

    return time[end, 1], join([join(path, "-") for path in criticalPaths], "/")
end

# Testovi za Fulkersonov algoritam

topologicalOrder([
    0 8 0 0 2 2;
    0 0 0 1 0 0;
    1 0 0 0 1 0;
    0 0 0 0 0 0;
    0 3 0 0 0 2;
    0 2 0 5 0 0;
]) == [3, 1, 5, 6, 2, 4]

topologicalOrder([
    0 0 0 0 0 0;
    1 0 0 0 0 0;
    5 2 0 0 0 0;
    0 3 1 0 0 0;
    0 8 3 2 0 0;
    0 0 0 5 1 0;
]) == [6, 5, 4, 3, 2, 1]

topologicalOrder([
    0 6 2 0 0 0;
    0 0 3 3 1 0;
    0 0 0 0 1 0;
    0 0 4 0 0 3;
    0 0 0 0 0 2;
    0 0 0 0 0 0;
]) == [1, 2, 4, 3, 5, 6]

topologicalOrder([
    0 3 5 0 0 0 0;
    0 0 0 2 3 0 0;
    0 0 0 0 0 8 0;
    0 0 7 0 2 3 0;
    0 0 0 0 0 0 1;
    0 0 0 0 0 0 4;
    0 0 0 0 0 0 0;
]) == [1, 2, 4, 3, 5, 6, 7]

topologicalOrder([
    0  2 10  7  0  0  0;
    0  0  0  3  9  0  0;
    0  0  0  6  0  6  0;
    0  0  0  0  5  8 12;
    0  0  0  0  0  1  7;
    0  0  0  0  0  0  4;
    0  0  0  0  0  0  0;
]) == [1, 2, 3, 4, 5, 6, 7]

# Testovi za Critical Path Method

# Testni primjer
cpm(
    ["A", "B", "C", "D", "E", "F", "G", "H", "I"],
    ["-", "-", "-", "C", "A", "A", "B,D", "E", "F,G"],
    [3, 3, 2, 2, 4, 1, 4, 1, 4]
) == (12, "C-D-G-I")
# Isti primjer, samo ispremetano
cpm(
    ["G", "I", "B", "C", "D", "A", "E", "F", "H"],
    ["B,D", "F,G", "-", "-", "C", "-", "A", "A", "E"],
    [4, 4, 3, 2, 2, 3, 4, 1, 1]
) == (12, "C-D-G-I")

# Udžbenik - Primjer 8.3
cpm(
    ["A", "B", "C", "D", "E", "F", "G"],
    ["-", "A", "B", "A", "D", "E", "C,F"],
    [25, 30, 60, 1, 50, 4, 6]
) == (121, "A-B-C-G")
# Isti primjer, samo ispremetano
cpm(
    ["E", "B", "D", "F", "A", "G", "C"],
    ["D", "A", "A", "E", "-", "C,F", "B"],
    [50, 30, 1, 4, 25, 6, 60]
) == (121, "A-B-C-G")

# Udžbenik - Primjer 8.4
cpm(
    ["1-2", "1-3", "1-4", "3-5", "3-6", "4-5", "4-7", "5-8", "6-7", "6-9", "7-10", "8-10", "8-12", "9-12", "10-11", "11-12"],
    ["-", "-", "-", "1-2,1-3", "1-2,1-3", "1-4", "1-4", "3-5,4-5", "3-6", "3-6", "4-7,6-7", "5-8,3-6", "5-8,3-6", "6-9", "7-10,8-10", "10-11"],
    [4, 2, 5, 7, 9, 8, 3, 2, 5, 4, 5, 4, 6, 5, 3, 8]
) == (34, "1-2-3-6-6-7-7-10-10-11-11-12")

# Udžbenik - Primjer 8.5
cpm(
    ["A", "B", "C", "D", "E", "F", "G", "H", "I"],
    ["-", "-", "-", "A", "A,B", "C", "D,E,F", "C", "H"],
    [2, 5, 4, 4, 2, 2, 4, 1, 2]
) == (11, "B-E-G")

# ZSR - Zadatak 1
cpm(
    ["A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K"],
    ["-", "-", "-", "A", "A", "C", "B,E", "D", "F", "F,G,H", "I,J"],
    [4, 10, 12, 6, 8, 8, 10, 10, 8, 10, 6]
) == (38, "A-E-G-J-K")

# ZSR - Zadatak 2
cpm(
    ["A", "B", "C", "D", "E", "F", "G", "H", "I"],
    ["-", "-", "-", "A", "A", "B,D", "B,D", "E,F", "B,C,D"],
    [5, 9, 6, 2, 4, 5, 8, 3, 11]
) == (20, "B-I")

# ZSR - Zadatak 3
cpm(
    ["A", "B", "C", "D", "E", "F", "G", "H", "I", "J"],
    ["-", "-", "A", "A", "B", "C", "C,D", "C", "F,G", "D,E"],
    [4, 5, 3, 5, 3, 4, 3, 7, 2, 5]
) == (14, "A-C-G-I/A-D-G-I/A-C-H/A-D-J")
# Kritični putevi u ZSR-u: A-C-H/A-D-H-I/A-D-J

# ZSR - Zadatak 4
cpm(
    ["A", "B", "C", "D", "E", "F", "G", "H"],
    ["-", "A", "A", "A", "A", "B,C", "D,E", "F,G"],
    [15, 30, 45, 45, 15, 15, 15, 15]
) == (90, "A-C-F-H/A-D-G-H")
# Isti primjer, samo ispremetano
cpm(
    ["F", "D", "E", "A", "C", "G", "B", "H"],
    ["B,C", "A", "A", "-", "A", "D,E", "A", "F,G"],
    [15, 45, 15, 15, 45, 15, 30, 15]
) == (90, "A-D-G-H/A-C-F-H")

# ZSR - Zadatak 5
cpm(
    ["A", "B", "C", "D", "E", "F", "G", "H"],
    ["-", "-", "-", "A", "A", "A,B", "D", "D,E,F"],
    [5, 4, 5, 1, 2, 7, 2, 2]
) == (14, "A-F-H")

# ZSR - Zadatak 6
cpm(
    ["A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K"],
    ["-", "-", "A", "A", "B", "B", "C", "D", "E", "G,H", "I,F"],
    [5, 4, 5, 1, 2, 7, 2, 2, 5, 6, 8]
) == (19, "B-E-I-K/B-F-K")
# Kritični putevi u ZSR-u: A-E-I-K/A-F-K