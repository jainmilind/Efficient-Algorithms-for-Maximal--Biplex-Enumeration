# Efficient-Algorithms-for-Maximal--Biplex-Enumeration
Parallel graph algorithms project 

# EnumAlmostSat

Initial solution (L, R) {found using brute force somehow}    input to our function
Add a vertex only in L                                       input to our function
(L U {v}, R)

(L'U{v}, R'), L' subset of L, R' subset of R

### 4.1
Each vertex u in R which is adj to v will be in our local solution = R_keep 
R - R_keep = R_enum
Just enumerate subsets of R_enum now for finding local solution
subset of R_enum is R''
So now R' = R'' U R_keep
size of R'' <= K 

### 4.2 : refine enumeration of R_enum
R_enum1 = all vertices with disconnections 𝛿(𝑢, 𝐿) <= k - 1
R_enum2 = R_enum - R_enum1 (exact K disconnections)

R_1'' subset of R_enum1
R_2'' subset of R_enum2

so if size of R_1'' U R_2'' < k then R_1'' = R_enum1


