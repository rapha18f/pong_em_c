# PONG! em C

Recriação do jogo PONG! em C dedicado a funcionar na versão de Sega Dreamcast, para fins de cumprimento acadêmico da displina Progamação I do curso de Engenharia da Computação na UFPB campus Mangabeira.

O código-fonte só funcionará se tiver o comando mkdcdisk (Make Dreamcast Disc) instalado, que por sua vez é disponibilizado apenas para sistemas Linux (ou WSL no Windows). Assim que for instalado, excute desse jeito: mkdcdisk *entrada_de_arquivo.c* *nome_desejado.cdi*

```bash
> mkdcdisk main.c main.cdi
```

Após isso, mova o arquivo pronto para a pasta que quer e instale o Redream na sua máquina para que rode (Flycast não funcionará) ou queimar o disco em CD caso você já tenha um Sega Dreamcast físico.
Redream: https://redream.io/

importe o disco nas configurações e aproveite o programa em execução! ;)
