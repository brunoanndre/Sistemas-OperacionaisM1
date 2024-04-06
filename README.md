**Projeto 1**
**Problemática:** uma Indústria de Alimentos de Santa Catarina chamada FoodSec S.A. possui a
tarefa de escanear alimentos por meio de câmeras e verificar se os mesmos estão corretos. Os
alimentos podem passar por uma das três esteiras disponíveis. As três esteiras são controladas
por um por um único computador centralizado. Esse computador recebe dados de um sensor
em cada uma das esteiras que captura a contagem de itens que são identificados como
seguros. A contagem é exibida em um display perto das esteiras (todos os displays são
controlados pela mesma lógica, é apenas uma replicação).
Diante disso, a empresa precisa que vocês implementem, por meio de aplicação para
distribuição Linux/Windows, uma solução que consiga realizar as contagens nas três esteiras e
exiba o resultado total (contagem esteira 1 + contagem esteira 2 + contagem esteira 3). A
empresa pede que seja simulado a solução em um sistema multicore com sistema operacional
com suporte a threads e IPC. A empresa solicita que um processo seja responsável pela
contagem usando threads e outro processo seja responsável pela apresentação no display.
Você deve usar pipe para que os dois processos troquem dados.
Além disso, os pesos dos itens que passam por cada uma das esteiras são armazenados em
um único vetor de dados. A cada 1.500 unidades de produtos, das três esteiras, é necessário
atualizar o peso total de itens processados. Sendo assim, a empresa aceita uma pausa na
quantidade de itens sendo contados e pesados para realizar a pesagem total.
A empresa também fornece uma análise das três esteiras:
• Esteira 1: produtos de maior peso (5 Kg) – passa 1 item a cada segundo pelo sensor.
• Esteira 2: produtos de peso médio (2 Kg) – passa 1 item a cada 0,5 segundo pelo sensor.
• Esteira 3: produtos de menor peso (0,5 Kg) – passa 1 item a cada 0,1 segundo pelo
sensor.
• A contagem deve acontecer initerruptamente.
• A exibição no display deve atualizar a cada 2 segundos para os operadores poderem
acompanhar.
• Um operador pode usar um botão no equipamento para poder parar a contagem devido
a um problema ocorrido.
Diante da problemática apresentada, vocês terão que implementar uma aplicação (em nível de
MVP) que possa lidar com tal situação usando duas abordagens: Pthreads e IPC entre
processos com pipe. No processo que utiliza threads, lembre-se de utilizar mutex ou semáforo
para controlar o acesso das seções críticas.
**Pontuação Extra na M1**
A utilização da biblioteca OpenMP para contagem do peso total usando o vetor de 1500 valores
irá receber de 0,5 à 1,5 pontos na prova da M1. A nota extra fica a critério do professor em
avaliar o contexto de aplicação, bem como a qualidade do código e aplicabilidade.
