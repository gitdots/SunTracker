DROP TABLE IF EXISTS Readings;

CREATE TABLE Readings (
    id INTEGER PRIMARY KEY,
    read_time DATETIME,  -- timestamp of the reading
    lul INTEGER,    -- intensity of ldr
    lur INTEGER,   
    ldl INTEGER,
    ldr INTEGER,
    panel FLOAT(2,1),
    temp FLOAT(2, 1),    -- temperature in degree Celsius
    hum FLOAT(2, 1) --,   -- relative humidity in percentage
);
 