// Polar Area Chart
var ctx;
var ctxData;
var polarAreaChart;

// Cartesian
var ctxCartesian;
var cartesianChart;

// Curent Line charts
var ctxCurrentAmp;
var ctxCurrentVolt;
var ctxCurrentWatt;

var currentAmpChart;
var currentVoltChart;
var currentWattChart;

var ctxTemp, ctxHum;

function initialize() {
    ctx = document.getElementById('polarChart').getContext('2d');
    ctxData = [0, 0, 0, 0];
    polarAreaChart = new Chart(ctx, {
        type: 'polarArea',
        data: {
            labels: ['Light Up Right', 'Light Down Right', 'Light Down Left', 'Light Up Left'],
            datasets: [{
                data: ctxData,
                backgroundColor: Chart.defaults.colorScheme,
            }],
            borderColor: Chart.defaults.colorScheme,
            borderWidth: 1
        },
        options: {
            responsive: true,
            scales: {
                r: {
                    beginAtZero: true,
                    min: 0,
                    max: 1024
                }
            }
        }
    });

    ctxCartesian = document.getElementById('cartesianChart').getContext('2d');
    cartesianChart = new Chart(ctxCartesian, {
        type: 'scatter',
        data: {
            datasets: [{
                label: 'Vertical Angle',
                data: [],
                backgroundColor: 'blue',
                pointRadius: 5
            }, {
                label: 'Horizontal Angle',
                data: [],
                backgroundColor: 'red',
                pointRadius: 5    
            }]
        },
        options: {
            scales: {
                x: {
                    type: 'linear',
                    position: 'bottom',
                    min: -100,
                    max: 100
                },
                y: {
                    min: -10,
                    max: 100
                }
            }
        }
    });

    let initialLabels = ['0', '-1', '-2', '-3', '-4', '-5', '-6', '-7', '-8', '-9'];
    var dataV = new Array(10).fill(0);
    var dataW = new Array(10).fill(0);
    var dataA = new Array(10).fill(0);

    ctxCurrentAmp = document.getElementById('amperageChart').getContext('2d');
    currentAmpChart = new Chart(ctxCurrentAmp, {
        type: 'line',
        data: {
            labels: initialLabels,
            datasets: [{
                label: 'Current [mA]',
                data: dataA,
                fill: false,
                backgroundColor: 'rgba(196, 34, 83, 0.8)',
                borderColor: 'rgba(196, 34, 83, 0.8)',
                borderWidth: 1,
                tension: 0.2
            }]
        },
        options: {
            scales: {
                beginAtZero: true,
                min: function() {
                    var minValue = Math.min(dataA);
                    return minValue - 0.1 * Math.abs(minValue);
                },
                max: function() {
                    var maxValue = Math.max(dataA);
                    return maxValue + 0.1 * Math.abs(maxValue);
                }
            }
        }
    });

    ctxCurrentVolt = document.getElementById('voltageChart').getContext('2d');
    currentVoltChart = new Chart(ctxCurrentVolt, {
        type: 'line',
        data: {
            labels: initialLabels,
            datasets: [{
                label: 'Voltage [V]',
                data: dataV,
                fill: false,
                backgroundColor: 'rgba(65, 24, 170, 0.8)',
                borderColor: 'rgba(65, 24, 170, 0.8)',
                borderWidth: 1,
                tension: 0.2
            }]
        },
        options: {
            scales: {
                y: {
                    beginAtZero: true,
                    max: 5
                }
            }
        }
    });

    ctxCurrentWatt = document.getElementById('wattageChart').getContext('2d');
    currentWattChart = new Chart(ctxCurrentWatt, {
        type: 'line',
        data: {
            labels: initialLabels,
            datasets: [{    
                label: 'Power [mW]',
                data: dataW,
                fill: false,
                backgroundColor: 'rgba(18, 102, 48, 0.8)',
                borderColor: 'rgba(18, 102, 48, 0.8)',
                borderWidth: 1,
                tension: 0.2
            }]
        },
        options: {
            responsive: true,
            scales: {
                // y: {
                //     beginAtZero: true,
                //     max: 1000.0
                // }
                beginAtZero: true,
                min: function() {
                    var minValue = Math.min(dataW);
                    return minValue - 0.1 * Math.abs(minValue);
                },
                max: function() {
                    var maxValue = Math.max(dataW);
                    return maxValue + 0.1 * Math.abs(maxValue);
                }
            }
        }
    });

    ctxTemp = document.getElementById('temperature');
    ctxHum = document.getElementById('humidity');
}


function updatePolarAreaChart(newData) {
    let lul = newData[0];
    let lur = newData[1];
    let ldl = newData[2];
    let ldr = newData[3];
    polarAreaChart.data.datasets[0].data = [lur, ldl, ldr, lul];
    polarAreaChart.update();
}

function updateCartesianChart(x, y) {
    cartesianChart.data.datasets[0].data = [];
    cartesianChart.data.datasets[1].data = [];
    cartesianChart.data.datasets[0].data.push({x:0, y:y});
    cartesianChart.data.datasets[1].data.push({x:x, y:0});
    cartesianChart.update();
}

function updateCurrentChart(chart, value) {
    chart.data.labels.pop();
    chart.data.datasets[0].data.pop();
    chart.data.labels.unshift(0);
    chart.data.datasets[0].data.unshift(value);
    for(let i = 1; i < chart.data.labels.length; i++) {
        chart.data.labels[i] = chart.data.labels[i] - 1;
    }
    chart.update();
}

function updateCurrentCharts(voltage, mAmps, mWatts) {
    updateCurrentChart(currentVoltChart, voltage);
    updateCurrentChart(currentAmpChart, mAmps);
    updateCurrentChart(currentWattChart, mWatts);
}

function updateTempHum(temp, hum) {
    ctxTemp.textContent = (parseFloat(temp.toFixed(2))).toString() + "°C";
    ctxHum.textContent = (parseFloat(hum.toFixed(2))).toString() + "%";
}