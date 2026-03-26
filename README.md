# Processador de Imagens SDL

## Integrantes do Grupo

- Bruna Aguiar Muchiuti - 10418358
- Gabriel Ken Kazama Geronazzo - 10418247
- Lucas Pires de Camargo Sarai - 10418013
- Jessica dos Santos Santana Bispo - 10410798

## Objetivo

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

Conceda permissão de execução aos scripts (necessário apenas na primeira vez):

```bash
$ chmod +x run.sh debug.sh
```

Utilize o script run.sh para compilar no modo `Release` e executar o projeto:

```bash
$ ./run.sh <path/to/image.png>
```

Exemplo de uso:

```bash
$ ./run.sh ../images/405.jpg
```

Ou, para compilar no modo `Debug` com sanitizers habilitados:

```bash
$ ./debug.sh <path/to/image.png>
```

Exemplo de uso:

```bash
$ ./debug.sh ../../images/405.jpg
```

## Resultados

### Imagens uniformes

#### Histograma
O histograma apresenta todos os pixels concentrados em um único nível de intensidade (0 ou 255). Isso indica ausência total de variação na imagem.
- Imagem branca → intensidades em 255 → imagem clara
- Imagem preta → intensidades em 0 → imagem escura
- Desvio padrão igual a zero → contraste inexistente
  
#### Equalização
A equalização não produz alterações visíveis. Como não há distribuição de intensidades, não existe informação para redistribuir.

![Descrição](docs/black1.jpg)
![Descrição](docs/black2.jpg)
![Descrição](docs/white1.jpg)
![Descrição](docs/white2.jpg)

---

### Imagem com boa distribuição

#### Histograma
Os níveis de intensidade estão distribuídos ao longo de toda a faixa (0–255).
- Média intermediária → imagem equilibrada (nem clara nem escura)
- Desvio padrão médio → contraste adequado
  
#### Equalização
A equalização redistribui levemente os níveis de intensidade, tornando a distribuição mais uniforme.

**Efeito:** 
- Pequeno aumento no contraste e melhor distinção entre tons.

![Descrição](docs/araras1.jpg)
![Descrição](docs/araras2.jpg)

---

### Imagem escura

#### Histograma
As intensidades estão concentradas em valores baixos.
- Média baixa → imagem escura
- Desvio padrão moderado → contraste limitado

#### Equalização
A equalização redistribui os níveis de intensidade ao longo de toda a faixa.

**Efeito:**
- A imagem se torna mais clara
- O contraste aumenta
- Detalhes antes pouco visíveis passam a ser perceptíveis

![Descrição](docs/401-1.jpg)
![Descrição](docs/405-2.jpg)

## Referências
