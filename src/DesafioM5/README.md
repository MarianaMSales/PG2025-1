# DesafioM5

## Funcionalidades

- **Personagem principal:** Menina animada usando spritesheet (Idle, Walk, Attack, Jump, Run)
- **Estados de animação:** parar, andar, atacar, pular, correr
- **Controles:**
  - **Direita:** D ou ➡️
  - **Esquerda:** A ou ⬅️
- **Atacar:** W ou ⬆️
- **Pular:** Espaço (pode pular e se mover ao mesmo tempo)
- **Correr:** S ou ⬇️ (segure junto com esquerda/direita)
- **Fundo parallax:** 6 camadas com diferentes velocidades
- **Apenas um personagem visível**

## Como executar

1. Compile o projeto usando CMake ou seu ambiente preferido.
2. Certifique-se de que as dependências (GLFW, GLAD, stb_image) estejam corretamente configuradas.
3. Execute o binário gerado (`DesafioM5.exe`).

## Estrutura dos arquivos

- `DesafioM5.cpp`: Código principal do projeto
- `assets/Layers/`: Imagens das camadas do fundo parallax
- `assets/sprites/`: Spritesheets da personagem

## Observações

- O personagem inicia parado (idle) e só anima/move quando uma das teclas permitidas é pressionada.
- O movimento durante o pulo é permitido (pressione espaço + A/D ou ⬅️➡️).

## Controles detalhados

| Tecla         | Ação                |
|---------------|---------------------|
| D / →         | Andar para direita  |
| A / ←         | Andar para esquerda |
| S / ↓         | Correr              |
| W / ↑         | Atacar              |
| Espaço        | Pular               |

