# Просто йонсольной чат с сервером на C++ и клиентом на Python
## Возможности
- Переписка в одной большой комнате
- Получение справочных данных
## Протокол сервера
- Принимает пакеты до 5 кб
- Первый байт означает тип операции (0 - отправить в общий чат, 1 - получить справочные данные)

## Как пользоваться клиентом
### Общение в чате
Для общения в чате вам нужно запустить клиент без аргумнтов.
### Получение справочных данных
Для получения справочных данных вам нужно в качестве аргумента указать их название(аргумент list выводит список названий)
