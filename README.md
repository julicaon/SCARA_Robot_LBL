# SCARA_Robot_LBL
В данном репозитории размещен проект по созданию робота для автоматизации создания сенсоров методом послойного наслаивания.

Для запуска программы:

1. Скачайте Arduino IDE на компьютер (https://www.arduino.cc/en/software/)
2. Установите библиотеки внутри Arduino IDE. AccelStepper и Servo для кореектной работы прграммы


3. Зайдите в программу и опуститесь до строки "НАСТРОЙКА ПРОГРАММЫ"
4. Поменяйте параметры программы, исходя из вашего эксперимента:
   SEMPLES - отвечает за количество образцов
   CIRCLES - отвечает за количество циклов для каждого из образцов
   RECIPE - отвечает за рецепт (ВАЖНО: Можно ввести ТОЛЬКО цифы от 1 до 6. Если ввести цифры больше 6 или меньше 1, то программа не будет работать)
   TUBE_TIME - отвечает за время опускания и высушивания

5. Нажмите на кнопку справа на верхней панели Arduino IDE
6. На появившемся интерфейсе в строке введите "start"