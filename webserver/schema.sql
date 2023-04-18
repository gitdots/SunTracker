DROP TABLE IF EXISTS Devices;
DROP TABLE IF EXISTS Readings;
DROP TABLE IF EXISTS DateSummary;

CREATE TABLE Devices (
    device_id INTEGER PRIMARY KEY, -- unique identifier for each device
    place VARCHAR(30) -- the location of the device
);

CREATE TABLE Readings (
    reading_time DATETIME,  -- timestamp of the reading
    device_id INTEGER,   -- the device that performed the reading
    ldr_up_left FLOAT(2, 2),    -- intensity of ldr
    ldr_up_right FLOAT(2, 2),   
    ldr_down_left FLOAT(2, 2),
    ldr_down_right FLOAT(2, 2),
    temperature FLOAT(2, 1),    -- temperature in degree Celsius
    humidity FLOAT(2, 1),   -- relative humidity in percentage
    FOREIGN KEY (device_id) REFERENCES Devices(device_id)
);

CREATE TABLE DateSummary (
    summary_date DATE PRIMARY KEY,
    device_id INTEGER,
    avg_temperature FLOAT(2, 1),
    avg_humidity FLOAT(2, 1),
    min_temperature FLOAT(2, 1),
    max_temperature FLOAT(2, 1),
    min_humidity FLOAT(2, 1),
    max_humidity FLOAT(2, 1),
    FOREIGN KEY (device_id) REFERENCES Device(device_id)
);
 
