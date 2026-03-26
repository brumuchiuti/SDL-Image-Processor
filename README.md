# Processador de Imagens SDL

## Integrantes do Grupo

- Bruna Aguiar Muchiuti - 10418358
- Gabriel Ken Kazama Geronazzo - 10418247
- Lucas Pires de Camargo Sarai - 10418013
- Jessica dos Santos Santana Bispo - 10410798

## Objetivo: TODO

## Funcionamento

**Visão geral da arquitetura:**

- **Camadas**
  - UI: renderização, gerenciamento de eventos e estado da aplicação
  - Backend: carregamento, processamento e análise de imagens

- **Componentes**
  - **UI**
    - `ui_components`: utilitários de UI e definições de componentes (botão e histograma) usados por `ui_view`.
    - `ui_view`: configuração e renderização de janelas e componentes.
    - `ui_state`: armazena dados e estado da aplicação.
    - `ui_controller`: coleta e processamento de eventos do usuário, atualizando o estado da aplicação conforme necessário.
  - **Backend**
    - `image_processor`: funções de processamento de imagem (conversão para escala de cinza, equalização de histograma, etc.).
    - `image_loader`: função de carregamento de imagens usando a biblioteca SDL_image.

![](docs/image-file-loaded.png)

> Fonte: https://freepascal-meets-sdl.net/rendering-image-files-with-any-format-sdl3_image/

## Contribuições

Apenas clone o repositório do projeto:

```bash
$ git clone git@github.com:brumuchiuti/SDL-Image-Processor.git
```

### O que cada integrante fez: TODO

## Compilação e Execução

Usando VS Code, aperte `CTRL+SHIFT+P` e então selecione :

```bash
> Tasks: Run Task
```

Em seguida, selecione o comando adequado ao seu sistema operacional (Windows ou Linux), ex:

```bash
> Compilar e executar Windows (MinGW: mingw32-make -> main.exe)
```

## Resultados: TODO

## Referências: TODO
