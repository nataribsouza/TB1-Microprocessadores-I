# Documentação do Display LCD 16x2

## Introdução
O display LCD 16x2 é amplamente utilizado em projetos de eletrônica para exibir informações alfanuméricas. Ele pode ser controlado diretamente por GPIOs ou por um módulo de interface I2C para reduzir o número de pinos necessários.

Este documento descreve os comandos básicos e como configurá-lo em modo 4 bits ou 8 bits.

---

## Especificações Básicas
- **Linhas**: 2
- **Colunas**: 16
- **Controlador Interno**: HD44780 ou compatível
- **Modos de Operação**: 4 bits ou 8 bits

---

## Pinos do Display LCD
| Pino | Nome        | Função                             |
| ---- | ----------- | ---------------------------------- |
| 1    | VSS         | Terra                              |
| 2    | VDD         | Alimentação (+5V)                  |
| 3    | V0          | Controle de contraste              |
| 4    | RS          | Registro de Seleção (Comando/Dado) |
| 5    | RW          | Leitura/Escrita                    |
| 6    | E           | Habilitação do Display             |
| 7-14 | D0-D7       | Barramento de Dados                |
| 15   | A (LED+)    | Alimentação do Backlight           |
| 16   | K (LED-)    | Terra do Backlight                 |

---

## Tabela de Comandos
Os comandos abaixo configuram e controlam o funcionamento do display LCD.

| Comando (Hex) | Descrição                               | Binário (D7-D0)   |
|---------------|-----------------------------------------|-------------------|
| `0x01`        | Limpar display                          | `0000 0001`       |
| `0x02`        | Retornar cursor para o início (home)    | `0000 0010`       |
| `0x04`        | Cursor move para a esquerda             | `0000 0100`       |
| `0x06`        | Incremento e shift desabilitado         | `0000 0110`       |
| `0x08`        | Display, cursor e blink desativados     | `0000 1000`       |
| `0x0C`        | Display ativado, cursor e blink off     | `0000 1100`       |
| `0x0E`        | Display ativado, cursor on, sem blink   | `0000 1110`       |
| `0x0F`        | Display ativado, cursor e blink on      | `0000 1111`       |
| `0x10`        | Shift do cursor para a esquerda         | `0001 0000`       |
| `0x14`        | Shift do cursor para a direita          | `0001 0100`       |
| `0x18`        | Shift do display para a esquerda        | `0001 1000`       |
| `0x1C`        | Shift do display para a direita         | `0001 1100`       |
| `0x20`        | Modo 4 bits                             | `0010 0000`       |
| `0x28`        | 4 bits, 2 linhas, 5x8 matriz            | `0010 1000`       |
| `0x38`        | 8 bits, 2 linhas, 5x8 matriz            | `0011 1000`       |
| `0x80 + addr` | Setar posição do cursor (addr = posição)| `1000 xxxx`       |

---

## Inicialização do Display (Modo 4 Bits)
1. Aguarde 15 ms após a alimentação do display.
2. Envie o comando `0x30` (3 vezes) com intervalos de 4.1 ms.
3. Envie o comando `0x20` para configurar o modo 4 bits.
4. Configure a função com `0x28`.
5. Ligue o display com `0x0C`.
6. Limpe o display com `0x01`.
7. Configure o incremento com `0x06`.

---

## Como Enviar Dados e Comandos
- **Dados**: RS = 1, RW = 0, enviar o byte correspondente.
- **Comandos**: RS = 0, RW = 0, enviar o byte correspondente.