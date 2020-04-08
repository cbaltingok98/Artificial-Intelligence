# Artificial-Intelligence

## Goal

Goal is to find the shortest possible path.

## Approach 

We will be generating random paths that has the same starting and ending point and paths that doesn't visit the same point more than once.

## Example Paths

1 -> 4 -> 6 -> 2 - > 8 -> 5 -> 3 -> 7-> 9 - > 1
7 -> 8 -> 3 -> 5 -> 9 -> 2 -> 1 -> 4 -> 6 -> 7

## Illegal Paths

3-> 8 -> 2 -> 8 -> 5 -> 9 -> 4 -> 6  -> 7 -> 3


* Populations [ 16, 32, 64]
* Generations [100, 200, 400]
* Default Path [1, 2, 3, 4, 5, 6, 7, 8, 9]

###Path Cost

|      1    2   3   4   5   6   7   8    9  |
----------------------------------
|  1   {  0      2   11     3   18   14   20   12     5  }
|  2   {  2      0   13   10     5     3     8   20   17  }
|  3   { 11   13     0     5   19   21     2     5     8  }
|  4   {  3    10     5     0     6     4   12   15     1  }
|  5   { 18     5   19     6     0   12     6     9     7  }
|  6   { 14     3   21     4   12     0   19     7     4  }
|  7   { 20     8     2   12     6   19     0   21   13  }
|  8   { 12   20     5   15     9     7   21     0     6  }
|  9   {  5   17      8     1     7     4   13     6     0  }

Initially, we are going to be creating 16 randomly generated paths using `Default Path`, each generated path's cost will be calculated and displayed in the result file. 

*  16 random path will be generated, 2 randomdly picked among 16 will be merged to create a new path. Meaning that we are going to have a children path from 2 parents and modify the children to get rid of the duplicate directions.

* From a population 16, a new population of 16 will be born 100 times. Each group of 16 is a generation. We are going to have 100 generations of the population 16.

* Each children path has a %20 chance of mutation, meaning that the order of the path will be randomly edited.

* After completing 100 generation of population 16, we will have a population of 32 with 200 generation, then a population of 64 with 400 generation. Reason to have different populations with different amount of generations is to see in which case we find the shortest path. 
