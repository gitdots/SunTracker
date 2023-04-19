DROP TABLE IF EXISTS Devices;
DROP TABLE IF EXISTS Readings;
DROP TABLE IF EXISTS DateSummary;

CREATE TABLE Devices (
    device_id INTEGER PRIMARY KEY, -- unique identifier for each device
    place VARCHAR(30) -- the location of the device
);

CREATE TABLE Readings (
    read_time DATETIME,  -- timestamp of the reading
    device_id INTEGER,   -- the device that performed the reading
    lul FLOAT(2, 2),    -- intensity of ldr
    lur FLOAT(2, 2),   
    ldl FLOAT(2, 2),
    ldr FLOAT(2, 2),
    temp FLOAT(2, 1),    -- temperature in degree Celsius
    hum FLOAT(2, 1),   -- relative humidity in percentage
    angle0x INTEGER,
    angle0y INTEGER,
    FOREIGN KEY (device_id) REFERENCES Devices(device_id)
);

CREATE TABLE DateSummary (
    summary_date DATE PRIMARY KEY,
    device_id INTEGER,
    min_temp FLOAT(2, 1),
    max_temp FLOAT(2, 1),
    min_hum FLOAT(2, 1),
    max_hum FLOAT(2, 1),
    FOREIGN KEY (device_id) REFERENCES Device(device_id)
);
 
