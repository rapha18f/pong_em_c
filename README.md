# PONG! em C

Recriação do jogo PONG! em C dedicado a funcionar na versão de Sega Dreamcast, para fins de cumprimento acadêmico da displina Progamação I do curso de Engenharia da Computação na UFPB campus Mangabeira.

O código-fonte só funcionará se tiver o comando mkdcdisk (Make Dreamcast Disc) e o dcbuild instalado, que por sua vez é disponibilizado apenas para sistemas Linux (Você pode usar DreamSDK incluindo interface gráfica caso você esteja no Windows).

## Instalação:
```bash
> sudo apt install docker #Caso não tenha instalado e seu sistema for Debian/Ubuntu
  sudo wget -O /usr/local/bin/dcbuild https://raw.githubusercontent.com/Nold360/docker-kallistios-sdk/master/dcbuild.sh
  sudo chmod +x /usr/local/bin/dcbuild
  sudo usermod -aG docker $USER
  sudo reboot
```
instalação do mkdcdisk

```bash
> #Escolha uma pasta para usar o arquivo abaixo do diretório builddir, em qualquer lugar.
  sudo apt install git meson build-essential pkg-config libisofs-dev
  git clone https://gitlab.com/simulant/mkdcdisc
  cd mkdcdisc

  meson setup builddir
  meson compile -C builddir

```
Assim que for instalado, já pode compilar o seu projeto (desde que tenha o Makefile desse jeito):

```
TARGET = <nome_do_objeto_compilado_que_desejas>.elf
# OBJS = src/main.o src/<outros_arquivos_que_vc_colocou_dentro_do_src>.o src/...

all: rm-elf $(TARGET)

include $(KOS_BASE)/Makefile.rules

clean:
	-rm -f $(TARGET) $(OBJS)

rm-elf:
	-rm -f $(TARGET)

$(TARGET): $(OBJS)
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) -o $(TARGET) $(KOS_START) \
		$(OBJS) $(OBJEXTRA) $(KOS_LIBS)

run: $(TARGET)
	$(KOS_LOADER) $(TARGET) -n

dist:
	rm -f $(OBJS)
	$(KOS_STRIP) $(TARGET)

#Altere esse Makefile na raiz do seu projeto assim que necessário.

Agora para a compilação do código:

```

```bash
> dcbuild make #na pasta raíz do projeto;
  pasta/do/mkdcdisk/builddir/mkdcdisk -e <arquivo>.elf -o <nome_do_arquivo>{.cdi|.iso} -n "<nome do jogo>"
```

Após de tudo disso, mova o arquivo pronto para a pasta que quer e instale o Redream na sua máquina para que rode (Flycast não funcionará) ou queimar o disco em CD caso você já tenha um Sega Dreamcast físico.
Redream: https://redream.io/

importe o disco nas configurações e aproveite os testes! Não esqueça-te de configurar os controles! ;)
