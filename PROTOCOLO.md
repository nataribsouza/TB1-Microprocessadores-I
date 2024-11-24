# Protocol Sensortech VOC sensor module 
Este documento demonstra o funcionamento do protocolo estabelecido entre o aspirador e o simulador, detalhando
o formato em que os comandos devem ser enviados e as respostas de cada dispositivo, tanto com os dados solicitados
quanto com a confirmação de comando recebido.

***
## Mensagens enviadas pelo microcontrolador


1. **Ligar aspirador**

Microcontrolador
| Data | Tipo   | Tamanho (bytes) | Descrição |
| ---- | ------ | --------------- | --------- |
| 'A'  | `char` | 1               | Remetente |
| 'A'  | `char` | 1               | Comando   |

Resposta simulador
| Data | Tipo   | Tamanho (bytes) | Descrição |
| ---- | ------ | --------------- | --------- |
| 'S'  | `char` | 1               | Remetente |
| 'A'  | `char` | 1               | Comando   |


2. **Desligar aspirador**

Microcontrolador
| Data | Tipo   | Tamanho (bytes) | Descrição |
| ---- | ------ | --------------- | --------- |
| 'A'  | `char` | 1               | Remetente |
| 'D'  | `char` | 1               | Comando   |

Resposta do simulador
| Data | Tipo   | Tamanho (bytes) | Descrição |
| ---- | ------ | --------------- | --------- |
| 'S'  | `char` | 1               | Remetente |
| 'D'  | `char` | 1               | Comando   |


3. **Ligar rodo (passapano)**

Microcontrolador
| Data | Tipo   | Tamanho (bytes) | Descrição |
| ---- | ------ | --------------- | --------- |
| 'A'  | `char` | 1               | Remetente |
| 'R'  | `char` | 1               | Comando   |

Resposta simulador
| Data | Tipo   | Tamanho (bytes) | Descrição |
| ---- | ------ | --------------- | --------- |
| 'S'  | `char` | 1               | Remetente |
| 'R'  | `char` | 1               | Comando   |


4. **Desligar rodo (passapano)**

Microcontrolador
| Data | Tipo   | Tamanho (bytes) | Descrição |
| ---- | ------ | --------------- | --------- |
| 'A'  | `char` | 1               | Remetente |
| 'P'  | `char` | 1               | Comando   |

Resposta simulador
| Data | Tipo   | Tamanho (bytes) | Descrição |
| ---- | ------ | --------------- | --------- |
| 'S'  | `char` | 1               | Remetente |
| 'P'  | `char` | 1               | Comando   |


5. **Movimentação do robô**

Microcontrolador
| Data | Tipo   | Tamanho (bytes) | Descrição |
| ---- | ------ | --------------- | --------- |
| 'A'  | `char` | 1               | Remetente |
| 'M'  | `char` | 1               | Comando   |
|  n   | `int`  | 1               | 1: segue en frente <br>2: vira à esquerda <br>3: vira à direita|

Resposta simulador
| Data | Tipo   | Tamanho (bytes) | Descrição         |
| ---- | ------ | --------------- | ----------------- |
| 'S'  | `char` | 1               | Remetente         |
| 'T'  | `char` | 1               | Comando           |
| te   | `int`  | 1               | Temperatura em °C |


6. **Solicita temperatura**

Microcontrolador
| Data | Tipo   | Tamanho (bytes) | Descrição |
| ---- | ------ | --------------- | --------- |
| 'A'  | `char` | 1               | Remetente |
| 'T'  | `char` | 1               | Comando   |

Resposta simulador
| Data | Tipo   | Tamanho (bytes) | Descrição         |
| ---- | ------ | --------------- | ----------------- |
| 'S'  | `char` | 1               | Remetente         |
| 'T'  | `char` | 1               | Comando           |
| te   | `int`  | 1               | Temperatura em °C |

***
## Mensagens enviadas pelo simulador


1. **Envia dados do apartamento e poeira coletada**

Simulador
| Data  | Tipo   | Tamanho (bytes) | Descrição |
| ----- | ------ | --------------- | --------- |
| 'S'   | `char` | 1               | Remetente |
| 'I'   | `char` | 1               | Comando   |
|  sf   | `int`  | 1               | 1: sem obstáculo <br>2: obstáculo à frente   |
|  col  | `int`  | 1               | Quantidade de poeira (g). Só coleta se ligado|

Resposta microcontrolador
| Data | Tipo   | Tamanho (bytes) | Descrição |
| ---- | ------ | --------------- | --------- |
| 'A'  | `char` | 1               | Remetente |
| 'I'  | `char` | 1               | Comando   |


2. **Envia horário**

Simulador
| Data  | Tipo   | Tamanho (bytes) | Descrição |
| ----- | ------ | --------------- | --------- |
| 'S'   | `char` | 1               | Remetente |
| 'H'   | `char` | 1               | Comando   |
|  hora | `int`  | 1               | Horas     |
|  min  | `int`  | 1               | Minutos   |

Resposta microcontrolador
| Data | Tipo   | Tamanho (bytes) | Descrição |
| ---- | ------ | --------------- | --------- |
| 'A'  | `char` | 1               | Remetente |
| 'H'  | `char` | 1               | Comando   |