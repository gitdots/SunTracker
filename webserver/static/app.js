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

function initialize() {
    ctx = document.getElementById('polarChart').getContext('2d');
    ctxData = [0, 0, 0, 0];
    polarAreaChart = new Chart(ctx, {
        type: 'polarArea',
        data: {
            labels: ['Light Up Left', 'Light Up Right', 'Light Down Left', 'Light Down Right'],
            datasets: [{
                data: ctxData,
                backgroundColor: Chart.defaults.colorScheme,
            }]
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
                label: 'mAmpers',
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
                y: {
                    beginAtZero: true,
                    max: 1.0
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
                label: 'Voltage',
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
                label: 'mWatts',
                data: dataW,
                fill: false,
                backgroundColor: 'rgba(18, 102, 48, 0.8)',
                borderColor: 'rgba(18, 102, 48, 0.8)',
                borderWidth: 1,
                tension: 0.2
            }]
        },
        options: {
            scales: {
                y: {
                    beginAtZero: true,
                    max: 1.0
                }
            }
        }
    });
}


function updatePolarAreaChart(newData) {
    polarAreaChart.data.datasets[0].data = newData;
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
    console.log("Update for voltage: " + value);
    chart.data.labels.pop();
    chart.data.datasets[0].data.pop();
    chart.data.labels.unshift(0);
    chart.data.datasets[0].data.unshift(value);
    console.log("volt: " + chart.data.datasets[0].data)
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