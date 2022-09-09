// index_html.h
const char INDEX_HTML[] PROGMEM = R"=====(
<!DOCTYPE html><html><head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
<title>Sensor graph</title>
<link rel="shortcut icon" href="/favicon.ico" />
</head>
<div style="text-align:center;"><b>Sensor graph</b></div>
<div class="chart-container" position: relative; height:350px; width:100%">
  <canvas id="myChart" width="600" height="400"></canvas>
</div>
<br><br>
<script src = "/Chart.min.js"></script>  
<script>
var graphData = {
  labels: [],  // X軸のデータ (時間)
  datasets: [
    {
        label: "Sensor-00",
        data: [], // Y軸のデータ(センシング結果)
        fill: false,
        borderColor : "rgba(97,132,254,0.8)",
        backgroundColor : "rgba(97,132,254,0.5)",
        
    },
    {
        label: "Sensor-01",
        data: [], // Y軸のデータ(センシング結果)
        fill: false,
        borderColor : "rgba(254,97,132,0.8)",
        backgroundColor : "rgba(254,97,132,0.5)",
        
    },
    {
        label: "Sensor-02",
        data: [], // Y軸のデータ(センシング結果)
        fill: false,
        borderColor : "rgba(132,254,97,0.8)",
        backgroundColor : "rgba(132,254,97,0.5)",
        
    }        

  ]
};
var graphOptions = {
  maintainAspectRatio: false,
  scales: {
    yAxes: [{
      ticks: {beginAtZero:true}
    }]
  }
};

var ctx = document.getElementById("myChart").getContext('2d');
var chart = new Chart(ctx, {
  type: 'line',
  data: graphData,
  options: graphOptions
});

var ws = new WebSocket('ws://' + window.location.hostname + ':81/');
ws.onmessage = function(evt) {
  var Time = new Date().toLocaleTimeString();
  var data_x0 = JSON.parse(evt.data)["val0"];
  var data_x1 = JSON.parse(evt.data)["val1"];
  var data_x2 = JSON.parse(evt.data)["val2"];
  var mode = JSON.parse(evt.data)["mode"];

  console.log(evt.data);

  console.log(Time);
  console.log(data_x0);
  console.log(data_x1);
  console.log(data_x2);

  if( chart.data.labels.push(Time) > 100 ){
    chart.data.labels.shift();
  }
  if( chart.data.datasets[0].data.push(data_x0) > 100 ){
    chart.data.datasets[0].data.shift();
  }
  if( chart.data.datasets[1].data.push(data_x1) > 100 ){
    chart.data.datasets[1].data.shift();
  }
  if( chart.data.datasets[2].data.push(data_x2) > 100 ){
    chart.data.datasets[2].data.shift();
  }

  
  var label_x0 = "000";
  var label_x1 = "111";
  var label_x2 = "222";

  if( mode == 0){
    label_x0 = "gyroX";
    label_x1 = "gyroY";
    label_x2 = "gyroZ";
  }
  if( mode == 3){
    label_x0 = "gyroVal";
    label_x1 = "motorCmd";
    label_x2 = "aveGain_x100";
  }  
  if( mode == 2){
    label_x0 = "minmaxX";
    label_x1 = "minmaxY";
    label_x2 = "minmaxZ";
  }  
  if( mode == 1){
    label_x0 = "gyroAveX";
    label_x1 = "gyroAveY";
    label_x2 = "gyroAveZ";
  }  

  chart.data.datasets[0].label = label_x0;
  chart.data.datasets[1].label = label_x1;
  chart.data.datasets[2].label = label_x2;



  chart.update();
};
ws.onclose = function(evt) {
  console.log("ws: onclose");
  ws.close();
}
ws.onerror = function(evt) {
  console.log(evt);
}
</script>
</body></html>
)=====";
