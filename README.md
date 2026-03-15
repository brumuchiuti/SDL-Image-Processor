# Processador de Imagens SDL

## Integrantes do Grupo

- Bruna Aguiar Muchiuti - 10418358
- Gabriel Ken Kazama Geronazzo - 10418247
- Lucas Pires de Camargo Sarai - 10418013
- Jessica dos Santos Santana Bispo - 10410798

## Objetivo

## Funcionamento

![](docs/image-file-loaded.png)

> Fonte: https://freepascal-meets-sdl.net/rendering-image-files-with-any-format-sdl3_image/

## Contribuições

1. Clone o repositório do projeto:

```bash
$ git clone --recursive git@github.com:brumuchiuti/SDL-Image-Processor.git
```

> [!NOTE]
> Após o clone, certifique-se de que o submódulo do SDL foi clonado corretamente. Se não, execute:
>
> ```bash
> $ git submodule update --init --recursive
> ```

2. Configure o projeto usando CMake:

```bash
$ cmake -S . -B build
```

2.1. Para habilitar os sanitizers, use a seguinte configuração:

```bash
$ cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_SANITIZERS=ON
```

> [!NOTE]
> Use o tipo de build `Debug` para garantir que os sanitizers funcionem corretamente, pois no modo `Release` as otimizações podem interferir na detecção de erros.

> [!WARNING] Important
> Toda vez que alterar alguma flag de configuração do CMake ou alterou o arquivo `CMakeLists.txt`, é necessário limpar o cache com `$ cmake --build build --target clean` e depois reconfigurar o projeto com os comandos da etapa `2.` ou `2.1.`.

## Compilação e Execução

Conceda permissão de execução ao script `run.sh` (necessário apenas na primeira vez):

```bash
$ chmod +x run.sh
```

Utilize o script run.sh para compilar e executar o projeto:

```bash
$ ./run.sh
```

## Resultados

## Referências
