#ifndef STRUCTS_H
#define STRUCTS_H

#include <cstdint>
inline constexpr struct SQLRequests
{
    char GetAirportList[109] = "SELECT airport_name->>'ru' as airportName, airport_code FROM bookings.airports_data ORDER BY airportName ASC";
    char GetArrivalPlanesDay[263] = "SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' as Name FROM bookings.flights f "
                            "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport "
                            "WHERE f.arrival_airport = ? AND scheduled_departure::date = date(?) ORDER BY scheduled_arrival ASC";
    char GetArrivalPlanesPeriod[307] = "SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' as Name FROM bookings.flights f "
                                    "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport "
                                    "WHERE f.arrival_airport = ? AND (scheduled_departure::date >= date(?) AND scheduled_departure::date <= date(?)) ORDER BY scheduled_arrival ASC";
    char GetDepartPlanesDay[265] = "SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' as Name FROM bookings.flights f "
                           "JOIN bookings.airports_data ad on ad.airport_code = f.arrival_airport "
                           "WHERE f.departure_airport = ? AND scheduled_departure::date = date(?) ORDER BY scheduled_arrival ASC";
    char GetDepartPlanesPeriod[309] = "SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' as Name FROM bookings.flights f "
                                   "JOIN bookings.airports_data ad on ad.airport_code = f.arrival_airport "
                                   "WHERE f.departure_airport = ? AND (scheduled_departure::date >= date(?) AND scheduled_departure::date <= date(?)) ORDER BY scheduled_arrival ASC";
    char AllStatsByDays[287] = "SELECT count(flight_no), date_trunc('day', scheduled_departure) as Day FROM bookings.flights f "
                              "WHERE (scheduled_departure::date > date('2016-08-31') AND scheduled_departure::date <= date('2017-08-31')) "
                              "AND (f.departure_airport = ? or f.arrival_airport = ?) GROUP BY Day ORDER BY Day ASC";
} requests;

enum requestType
{
    requestAirportList = 1,
    requestArrivalPlanes,
    requestDepartPlanes,
    requestAllStats,
    requestAllStatsByDays
};

#define CONN_INTERVAL 5000
#define POSTGRE_DRIVER "QPSQL"
#define DB_NAME "FlightDB"
#define RESTRICT_DATE_FROM "15.08.2016"
#define RESTRICT_DATE_TO "14.09.2017"

inline constexpr struct DataBaseParams
{
    char dbName[10] = "demo";
    char user[17] = "netology_usr_cpp";
    char pass[9] = "CppNeto3";
    char host[24] = "981757-ca08998.tmweb.ru";
    uint16_t port = 5432;
} dbParams;

inline constexpr struct DataBaseInfoStatus
{
    char Connected[21] = "Подключено";
    char Disconnected[19] = "Отключено";
    char Connection[26] = "Подключение...";
    char ErrorConnection[37] = "Ошибка подключения!";
    char TerminateConnection[88] = "Подключение разорвано! Попопытка подключения...";
    char ErrorSQLrequest[33] = "Ошибка SQL запроса!";
    char ExecuteSQLrequest[43] = "Выполнение SQL запроса...";
    char FlightsByDate[71] = "Выборка рейсов осуществляется за дату";
    char FlightsByPeriod[75] = "Выборка рейсов осуществляется за период";
    char WrongPeriod[65] = "Дата С не может быть больше Даты По!";
    char OK[3] = "OK";
} dbInfo;


#endif // STRUCTS_H
