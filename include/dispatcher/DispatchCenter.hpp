#pragma once                              // Предотвращает многократное подключение этого заголовочного файла
#include "dispatcher/DispatchUnit.hpp"    // Подключает описание диспетчерского подразделения (единицы)
#include "dispatcher/Incident.hpp"       // Подключает описание инцидента (происшествия)
#include "dispatcher/EventLog.hpp"       // Подключает описание журнала событий
#include <vector>                         // Подключает стандартную библиотеку для работы с динамическими массивами

namespace dispatcher {                    // Пространство имён для компонентов диспетчерской системы

class DispatchCenter {                    // Центр диспетчеризации — главный класс управления единицами и инцидентами
public:
    explicit DispatchCenter(EventLog& log);  // Конструктор: инициализирует центр с переданным журналом событий

    void addUnit(DispatchUnit unit);          // Добавляет диспетчерское подразделение в систему
    void addIncident(Incident incident);      // Добавляет новый инцидент в список отслеживаемых

    bool assign(int unitId, int incidentId);   // Назначает подразделение на обработку инцидента (возвращает успех операции)
    bool closeIncident(int incidentId);       // Закрывает указанный инцидент (возвращает успех операции)

    void printUnits() const;                  // Выводит список всех подразделений
    void printIncidents() const;              // Выводит список всех инцидентов
    void printState() const;                 // Выводит текущее состояние диспетчерского центра

private:
    DispatchUnit* findUnit(int id);           // Ищет подразделение по идентификатору (возвращает указатель)
    Incident* findIncident(int id);           // Ищет инцидент по идентификатору (возвращает указатель)

    EventLog& log_;                           // Ссылка на журнал событий для записи действий
    std::vector<DispatchUnit> units_;        // Список всех зарегистрированных подразделений
    std::vector<Incident> incidents_;        // Список всех активных инцидентов
};

} // namespace dispatcher