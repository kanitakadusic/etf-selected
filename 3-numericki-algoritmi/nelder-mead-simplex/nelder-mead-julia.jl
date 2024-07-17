using LinearAlgebra

"""
 - `n` number of vertices
 - `d` size of simplex
 - `return` d * [0 ; E]
"""
function getSimplex(n, d = 1.0)
    [i - 1 == j ? d : 0.0 for i in 1 : n, j in 1 : n - 1]
end

# domain convergence test
# becomes true when the working simplex is sufficiently small i.e. all vertices are close enough
function domainTestPassed(simplex, tolerance)
    all(abs.(simplex[1, 1 : end] .- simplex[2 : end, 1 : end]) .<= tolerance)
end

# function-value convergence test
# becomes true when the values of the function in all vertices of the simplex are close enough
function valueTestPassed(f, simplex, tolerance)
    all(abs.(f(simplex[1, :]) .- f.(eachrow(simplex[2 : end, :]))) .<= tolerance)
end

"""
 - `f` function with dimension >= 2
 - `simplex` n /vertices/ x (n - 1) /function dimension/
 - `tolerance` non-negative
 - `maxIterations` non-negative
 - `return` [accuracy achieved, minimum]
"""
function NelderMead(f, simplex, tolerance = 1e-10, maxIterations = 500)
    if size(simplex, 1) < 3 || any(length(vertex) != size(simplex, 1) - 1 for vertex in eachrow(simplex))
        throw(ArgumentError("simplex"))
    end

    if tolerance < 0 || maxIterations < 0
        throw(ArgumentError("tolerance or maxIterations"))
    end

    # N - number of vertices of the simplex
    # D - dimension of the function
    N, D = size(simplex, 1), size(simplex, 2)
    
    # iBest - index of the best performing vertex
    # iWorst - index of the worst performing vertex
    # iSecondWorst - index of the second worst performing vertex
    iBest, iWorst, iSecondWorst = 1, N, N - 1
    
    # ALPHA - parameter for reflection
    # BETA - parameter for contraction
    # GAMMA - parameter for expansion
    # DELTA - parameter for shrinkage
    ALPHA, BETA, GAMMA, DELTA = 1.0, 0.5, 2.0, 0.5
    
    while maxIterations > 0
        # sort the vertices by corresponding function values in ascending orderplease
        # f(simplex[1, :]) < f(simplex[2, :]) < ... < f(simplex[N, :])
        for i in 2 : size(simplex, 1)
            j = i
            
            while j > 1 && f(simplex[j, :]) < f(simplex[j - 1, :])
                simplex[j, :], simplex[j - 1, :] = simplex[j - 1, :], simplex[j, :]
                j -= 1
            end
        end

        if domainTestPassed(simplex, tolerance) || valueTestPassed(f, simplex, tolerance)
            break
        end

        # calculate the centroid of the best side - the one opposite the worst vertex
        # the worst vertex is the last one after sorting
        centroid = sum(simplex[1 : N - 1, :], dims = 1) / (N - 1)

        # compute the reflection point
        reflection = [centroid[i] + ALPHA * (centroid[i] - simplex[iWorst, i]) for i in 1 : D]
        
        if f(reflection) < f(simplex[iSecondWorst, :])
            # the reflection point is better than the second worst point
            # check if it is better than the best point
            if f(reflection) < f(simplex[iBest, :])
                # the reflection point is better than all of the vertices
                # compute the expansion point
                expansion = [centroid[i] + GAMMA * (reflection[i] - centroid[i]) for i in 1 : D]
                
                # accept the expansion point if it is better than the reflection point
                # otherwise accept the reflection point
                if f(expansion) < f(reflection)
                    reflection .= expansion
                end                
            end
            
            simplex[iWorst, :] .= reflection
        else
            # the reflection point is as good as or worse than the second worst point
            # check if it is better than the worst point
            isContracted = false

            if f(reflection) < f(simplex[iWorst, :])
                # the reflection point is better than the worst point
                # compute the outsideContraction point
                contraction = [centroid[i] + BETA * (reflection[i] - centroid[i]) for i in 1 : D]

                # accept the outsideContraction point if it is better or as good as the reflection point
                # otherwise perform a shrink transformation by computing n new vertices
                if f(contraction) <= f(reflection)
                    isContracted = true
                end
            else
                # the reflection point is worse than all of the points
                # compute the insideContraction point
                contraction = [centroid[i] + BETA * (simplex[iWorst, i] - centroid[i]) for i in 1 : D]
                
                # accept the insideContraction point if it is better than the worst point
                # otherwise perform a shrink transformation by computing n new vertices
                if f(contraction) < f(simplex[iWorst, :])
                    isContracted = true
                end
            end

            # perform contraction or shrinkage
            if isContracted
                simplex[iWorst, :] .= contraction
            else
                for i in 2 : N
                    simplex[i, :] .= simplex[iBest, :] .+ DELTA * (simplex[i, :] .- simplex[iBest, :])
                end
            end
        end
        
        maxIterations -= 1
    end

    return maxIterations != 0, simplex[iBest, :]
end

using Printf

function generateTests()
    # function name, function
    # point of minimum, value at minimum
    return [
        ("Paraboloid", x -> (x[1] + 5) ^ 2 + (x[2] + 2) ^ 2, 
            [-5.0, -2.0], 0.0),
        ("Sphere", x -> sum(x .^ 2), 
            [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0], 0.0),
        ("Rosenbrock", x -> sum(100 * (x[i + 1] - x[i] ^ 2) ^ 2 + (1 - x[i]) ^ 2 for i in 1 : length(x) - 1), 
            [1.0, 1.0, 1.0, 1.0, 1.0], 0.0),
        ("Beale", x -> (1.5 - x[1] + x[1] * x[2]) ^ 2 + (2.25 - x[1] + x[1] * x[2] ^ 2) ^ 2 + (2.625 - x[1] + x[1] * x[2] ^ 3) ^ 2, 
            [3.0, 0.5], 0.0),
        ("Booth", x -> (x[1] + 2 * x[2] - 7) ^ 2 + (2 * x[1] + x[2] - 5) ^ 2, 
            [1.0, 3.0], 0.0),
        ("Matyas", x -> 0.26 * (x[1] ^ 2 + x[2] ^ 2) - 0.48 * x[1] * x[2], 
            [0.0, 0.0], 0.0),
        ("Easom", x -> -cos(x[1]) * cos(x[2]) * exp(- (x[1] - pi) ^ 2 - (x[2] - pi) ^ 2), 
            [pi, pi], -1.0),
        ("McCormick", x -> sin(x[1] + x[2]) + (x[1] - x[2]) ^ 2 - 1.5 * x[1] + 2.5 * x[2] + 1, 
            [-0.54719, -1.54719], -1.9133)
    ]
end

function NelderMeadTest()
    accurate = generateTests()

    for i in 1 : length(accurate)
        tested = NelderMead(accurate[i][2], getSimplex(length(accurate[i][3]) + 1), 1e-12)

        @printf("%15s%-10s", "FUNCTION", "")
        println(accurate[i][1])
        @printf("%15s%-10s", "ACCURATE", "")
        println(accurate[i][3], " = ", accurate[i][4])
        @printf("%15s%-10s", "TRUE/FALSE", "")
        println(tested[1])
        @printf("%15s%-10s", "TESTED", "")
        println(tested[2], " = ", accurate[i][2](tested[2]), "\n")
    end
end

NelderMeadTest()