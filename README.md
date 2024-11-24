# TB1-Microprocessadores-I


Este projeto tem como objetivo a implementação de um firmware embarcado em microcontrolador AVR ATMega-328
que deverá simular o comportamento de um robô aspirador.
O microcontrolador se comunica via USART com um computador que roda o software de simulação. Esse software
envia ao microcontrolador as informações referentes ao ambiente. A partir dessas informações, o microcontrolador
deve tomar decisões e executar a sua rotina de limpeza.
***

## Requisitos de projeto
O projeto apresenta diversos requisitos referentes ao comportamento do robô. Esses requisitos estão elencados
abaixo.

***
1. **Periféricos**
- O projeto deve possuir teclado alfanumérico matricial, LED indicativo de tanque cheio e display LCD 16x2.
- O teclado alfanumérico possui 6 botões organizados em 3 colunas e 2 linhas.

|  1  |  2  |  3  |
| --- | --- | --- |
|  4  |  5  |  #  |

***
2. **Movimentação**
- O robô deverá-se se movmentar de forma a evitar colisões com móveis e paredes. Para isso, o robô deve solicitar 
ao software de simulação os dados do sensor de proximidade, que informa se há ou não um obstáculo à frente.

***
3. **Limpeza**
- O robô conta com sucção de poeira, vassouras rotativas e passagem de pano úmido.
- O reservatório de sujeira do aspirador possui capacidade para 250g. Caso o reservatório esteja acima de 90% de sua capacidade, o LED de indicação deverá piscar em frequência 2 Hz e exibir essa informação no display LCD.

***
4. **Bateria**
- O robô possui uma bateria de 10mAh (600mA x min). A tabela a seguir esquematiza o consumo do robô em diferentes
operações.

| Modo de operação  |  Consumo  |
| ----------------- | --------- |
| Ligado            |  30 mA/s  |
| Aspirando         | +180 mA/s |
| Rodo (passa-pano) | +120 mA/s |

- O robô deve retornar à base automaticamente antes que a bateria acabe.

***
5. **Modos de limpeza**
- Limpeza automática: limpa todo o espaço disponível
- Limpeza de cantos: limpa o perímetro do ambiente, deslocando-se continuamente em um mesmo sentido.

Por padrão, o robô deve ligar todos os dias às 9h e fazer a limpeza automática.
Esse horário pode ser trocado pelo usuário administrador.

***
6. **Usuários**
- O robô deve prever interassão com dois usuários externos.
- Usuário 1 (senha 1325): esse usuário possui configurar parâmetros do sistema
- Usuário 2 (senha 4213): esse usuário pode momentaneamente selecionar o tipo de limpeza dos aspirador, mas não pode
alterar a configuração padrão.

***
7. **Função PlayDead**
- Com o intuito de evitar ataques felinos, caso o microcontrolador observe uma variação brusca de temperatura, ele deve
parar sua movimentação e esperar o perigo passar.

***
8. **Definição de barreiras (Grupos B)**
- Deve ser permitido ao administrador definir até duas barreiras virtuais por onde o robô não deve passar.
- Essa barreiras possuem o formato de retângulos e são definidas pelo ponto central, sua largura X e sua largura Y.