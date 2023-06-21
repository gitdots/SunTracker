var dateSelected;
var periodSelectedStart = new Date();
var periodSelectedEnd = new Date();
var totalPowerContainer;

$(function () {
    $("#daypicker").datepicker();
    $("#datepickerRangeStart").datepicker();
    $("#datepickerRangeEnd").datepicker();

    $('input[type=radio][name=dateSelectionType]').change(function () {
        if (this.value == 'specificDay') {
            $('#specificDayPicker').show();
            $('#dateRangePicker').hide();
        } else if (this.value == 'range') {
            $('#specificDayPicker').hide();
            $('#dateRangePicker').show();
        }
    });
    
    totalPowerContainer = $('#powerContainer');
});


var ctxPower = document.getElementById('powerChartC').getContext('2d');
var powerChart = new Chart(ctxPower, {
    type: 'line',
    data: {
        labels: ['00:00', '01:00', '02:00', '03:00', '04:00', '05:00', '06:00', '07:00', '08:00', '09:00', '10:00', '11:00', 
                '12:00', '13:00', '14:00', '15:00', '16:00', '17:00', '18:00', '19:00', '20:00', '21:00', '22:00', '23:00', '24:00'],
        datasets: [{
            label: 'Power [mW]',
            data: [],
            backgroundColor: 'rgba(18, 102, 48, 0.8)',
            borderColor: 'rgba(18, 102, 48, 0.8)',
            borderWidth: 1,
            fill: false,
            tension: 0.2
        }]
    },
    options: {
        maintainAspectRatio: false,
        scales: {
            x: {
                beginAtZero: true
            },
            y: {
                beginAtZero: true
            }
        }
    }
});

var ctxTemperature = document.getElementById('tempChartC').getContext('2d');
var tempChart = new Chart(ctxTemperature, {
    type: 'line',
    data: {
        labels: ['00:00', '01:00', '02:00', '03:00', '04:00', '05:00', '06:00', '07:00', '08:00', '09:00', '10:00', '11:00', 
                '12:00', '13:00', '14:00', '15:00', '16:00', '17:00', '18:00', '19:00', '20:00', '21:00', '22:00', '23:00', '24:00'],
        datasets: [{
            label: 'Temperature [°C]',
            data: [],
            backgroundColor: 'rgba(196, 34, 83, 0.8)',
            borderColor: 'rgba(196, 34, 83, 0.8)',
            borderWidth: 1,
            fill: false,
            tension: 0.2
        }]
    },
    options: {
        maintainAspectRatio: false,
        scales: {
            x: {
                beginAtZero: true
            },
            y: {
                beginAtZero: true
            }
        }
    }
});

var ctxHumidity = document.getElementById('humChartC').getContext('2d');
var humChart = new Chart(ctxHumidity, {
    type: 'line',
    data: {
        labels: ['00:00', '01:00', '02:00', '03:00', '04:00', '05:00', '06:00', '07:00', '08:00', '09:00', '10:00', '11:00', 
                '12:00', '13:00', '14:00', '15:00', '16:00', '17:00', '18:00', '19:00', '20:00', '21:00', '22:00', '23:00', '24:00'],
        datasets: [{
            label: 'Humidity [%]',
            data: [],
            backgroundColor: 'rgba(65, 24, 170, 0.8)',
            borderColor: 'rgba(65, 24, 170, 0.8)',
            borderWidth: 1,
            fill: false,
            tension: 0.2
        }]
    },
    options: {
        maintainAspectRatio: false,
        scales: {
            x: {
                beginAtZero: true
            },
            y: {
                beginAtZero: true
            }
        }
    }
});

function updateHumidityChart(timestamps, humData) {
    humChart.data.labels = timestamps;
    humChart.data.datasets[0].data = humData;
    humChart.update();
}

function updateTemperatureChart(timestamps, tempData) {
    tempChart.data.labels = timestamps;
    tempChart.data.datasets[0].data = tempData;
    tempChart.update();
}

function timestampToMinutes(timestamp) {
    let [hour, minute] = timestamp.split(":");
    return parseInt(hour) * 60 + parseInt(minute);
}

function updatePowerChart(timestamps, powerData) {
    let powerDataWatt = powerData.map(mw => mw / 1000);
    let totalPowerGenerated = 0;
    for(let i = 0; i < timestamps.length - 1; i++) {
        let [timeHour, timeMin] = timestamps[i].split(':'); 
        let [nextHour, nextMin] = timestamps[i+1].split(':');

        let timeDiff = (nextHour - timeHour) * 60 + (nextMin - timeMin);

        // if(timeDiff > 15) {
        //     totalPowerGenerated += 0;
        // }
        // else {
        const avgPower = (powerDataWatt[i] + powerDataWatt[i+1]) / 2;
        timeDiff = timeDiff / 60;
        // console.log('From ', timestamps[i], ' until ', timestamps[i+1], ': ', timeDiff, ', ', avgPower);
        const powerGenerated = avgPower * timeDiff;
        totalPowerGenerated += powerGenerated;
        // }
    }

    const powerText = "Power generated: " + totalPowerGenerated.toFixed(2).toString() + 'Watt-hour';
    totalPowerContainer.text(powerText);

    powerChart.data.labels = timestamps;
    powerChart.data.datasets[0].data = powerData;

    powerChart.update();
}

function requestDayData(date) {
    let dateOnly = date.getFullYear() + '-' + ('0' + date.getMonth()).slice(-2) + '-' + ('0' + date.getDate()).slice(-2);
    var urlData = {};
    urlData.type = 'day';
    urlData.day = dateOnly;
    
    console.log('Send requrest for: ', urlData.day);
    $.getJSON('/get_date_data', urlData, function (recData) {
        let timestamps = recData.map(function(item) {
            return item.timestamp;
        });
        let powerData = recData.map(function(item) {
            return item.power;
        });
        let tempData = recData.map(function(item) {
            return item.temp;
        });
        let humData = recData.map(function(item) {
            return item.hum;
        });
        
        console.log(timestamps, '    ', powerData);

        updatePowerChart(timestamps, powerData);
        updateHumidityChart(timestamps, humData);
        updateTemperatureChart(timestamps, tempData);
    });
}

function requestPeriodData(start, end) {
    while (start <= end) {
        let urlData = {
            type: 'day',
            day: start.getFullYear() + '-' + ('0' + start.getMonth()).slice(-2) + '-' + ('0' + start.getDate()).slice(-2)
        };
        
        console.log('Requested for: ', urlData);
        $.getJSON('/get_date_data', urlData)
            .done(function(recData) {
                
                console.log(recData);
            })
            .fail(function(jqxhr, textStatus, error) {
                var err = textStatus + ", " + error;
                console.log("Request Failed: " + err );
            });
    
        start.setDate(start.getDate() + 1);
    }
}

function formatDate(dateToBeFormatted) {
    let [month, day, year] = String(dateToBeFormatted).split('/');
    let date = new Date(year, month, day);
    return date;
}

let getData = document.getElementById('getDataButton').addEventListener('click', function () {
    var selectionType = $('input[type=radio][name=dateSelectionType]:checked').val();
    if (selectionType == 'specificDay') {
        var date = $('#daypicker').val();
        if (date === "") {

        } else {
            requestDayData(formatDate(date));
        }
    } else if (selectionType == 'range') {
        var startDate = $('#datepickerRangeStart').val();
        var endDate = $('#datepickerRangeEnd').val();
        if (startDate === "" || endDate === "") {

        } else {
            requestPeriodData(formatDate(startDate), formatDate(endDate));
        }
    }
});

