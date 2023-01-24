# Tetravex [![Profile][title-img]][profile]

[title-img]:https://img.shields.io/badge/-SCIA--PRIME-red
[profile]:https://github.com/Pypearl

### Authors

* Alexandre Lemonnier
* Victor Simonin

---

### Objectives

In this project, the goal was to solve a combinatorial problem. To do this, we proposed an implementation of a solver to solve puzzles of the Tetravex game. The goal of the Tetravex game is to solve a puzzle consisting of placing square tiles numbered from 1 to 9 in a square grid and that all the numbers on the edges of the tiles are equal to those adjacent.

<img src="https://github.com/Pypearl/CMKV/blob/main/images/tetravex.png" alt="Range plot">

---

### Metropolis-Hastings algorithm

The Metropolis-Hastings algorithm is based on statistical mechanics and uses simulation methods to find optimal solutions. It is often used to solve problems that are difficult to solve analytically. The main idea of the Metropolis-Hastings algorithm is to define an energy function for each possible configuration of a system, and to search for the minimum energy configuration that corresponds to the solution of that system.

---

### Performance

The initial goal of this project is to create an intelligent algorithm that uses statistical methods seen in class to be able to solve a complex problem in a simple and especially as quickly as possible.
Here the complexity of this algorithm depends on the complexity of the evaluation of the target density function and the generation of a sample of the transition distribution. In general, the complexity can be improved by using more efficient transition distributions, such as Gaussian distributions, and by using methods to accelerate convergence, such as transition distribution adaptation techniques.

---

### Usage

Any Tetravex examples could be tested using the following commands :

```bash
g++ src/*.cc -DNDEBUG -O5 -std=c++17 -lpthread -Wl,--no-as-needed
python script.py [--folder] [--file] input expected_output
```
