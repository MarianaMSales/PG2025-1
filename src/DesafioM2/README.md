# Desafio do Módulo 2

## 📌 Descrição da tarefa

---

### 🧩 PARTE 1 – Sem matriz de transformação  
**Arquivo:** `src/DesafioM2/Exercicio1.cpp`

#### ✅ Exercício 1  
Crie uma função:

```cpp
GLuint createTriangle(float x0, float y0, float x1, float y1, float x2, float y2);
```

Essa função deve criar um triângulo com as coordenadas dos vértices `v0`, `v1` e `v2` passadas por parâmetro, retornando seu identificador de VAO.

#### ✅ Exercício 2  
Instancie **5 triângulos** na tela utilizando a função criada anteriormente.

---

### 🧩 PARTE 2 – Com matriz de transformação  
**Arquivo:** `src/DesafioM2/Exercicio2.cpp`

> A partir deste exercício, sugere-se utilizar a biblioteca **GLM** para cálculos matemáticos.

#### ✅ Exercício 3  
Crie uma estrutura `Triangle` (`struct` ou `class`) que armazene:

- A posição do triângulo (x, y)
- A cor do triângulo (RGB)

Utilizando a função criada anteriormente, gere **um único VAO** para um triângulo padrão com os seguintes vértices:

```text
v0(-0.1, -0.1)  
v1(0.1, -0.1)  
v2(0.0, 0.1)
```

Com isso, seu programa deve:

- Utilizar um `array`, `vector` ou `list` de estruturas `Triangle`;
- Criar novos triângulos posicionados com base no clique do mouse;
- Sortear valores para as componentes RGB da cor de cada triângulo.
