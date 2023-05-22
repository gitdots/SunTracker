// Polar Area Chart
var ctx;
var ctxData;
var polarAreaChart;

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
}

function updatePolarAreaChart(newData) {
    polarAreaChart.data.datasets[0].data = newData;
    polarAreaChart.update();
}

// Cross Pins
var pinH = document.getElementById('pinH');
var pinV = document.getElementById('pinV');

function adjustPins(horizontalValue, verticalValue) {
    // Normalize values
    var normalizedH = (horizontalValue + 90) / 180;
    var normalizedV = (verticalValue + 90) / 180;

    // Position pins
    pinH.style.left = normalizedH * 100 + '%';
    pinV.style.top = (1 - normalizedV) * 100 + '%'; // invert vertical value to match the HTML coordinate system
}

// Test the function
// adjustPins(10, -30);
