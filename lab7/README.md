# Лабораторная работа №7

Была поставлена задача разработать функцию, которая принимает строку и меняет 
регистр через модификацию 6-ого бита. Для ускорения предлагается читать строку 
из 32/64 бит, а не символ за один раз, если длина строки кратна 4/8 байт. 

В результате работы была разработана программа, использующая искомую функцию. В 
целях оптимизации по памяти она считывает не строку целиком, а работает со вводом
по частям по 8 байт каждая. 

Поскольку при вводе из tty могут возникнуть проблемы с EOF (если длина ввода кратна 
восьми, то нужно дважды подавать сигнал Ctrl-D), программа считывает символы по одному, 
если обнаруживает, что `stdin` принадлежит tty. Также в tty программа останавливает чтение
строки на символе `\n` для удобства.

Применение битовой маски для всего блока в памяти затрудняется тем, что ввод может состоять
не только из букв.