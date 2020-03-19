var but = []; 
var mes = []; 
var defaultLang;
var restartMessage;			
window.onload = function() {  
    openValues(); 
    liveValues();
    init();
    updateChart();	
}
function changeLang(selectedLang) {
	event.preventDefault();
    openlang(selectedLang);
	var jsonData = 'defaultLang=' + selectedLang;
	var xhttp = new XMLHttpRequest();
	xhttp.open("GET", "lang?"+jsonData, true);	
	xhttp.send();	
}
function openlang(lng) {
    var path = lng+'.json';
	var xhttp = new XMLHttpRequest();
	xhttp.open("GET", path, true);
	xhttp.onreadystatechange = function() {
		if (this.readyState == 4 && this.status == 200) {
			var jsonfile = JSON.parse(this.responseText);
				document.querySelector("label[for=tab_0").innerHTML = jsonfile.tab[0];
				document.querySelector("label[for=tab_1").innerHTML = jsonfile.tab[1];
				document.querySelector("label[for=tab_2").innerHTML = jsonfile.tab[2];
				document.querySelector("label[for=tab_3").innerHTML = jsonfile.tab[3];				
			for (i in jsonfile.text_h){
				var text_arr = "text"+(parseInt(i));
				document.getElementById(text_arr).innerHTML = jsonfile.text_h[i];
			}			
			for (i in jsonfile.text_sh){
				var text_arr = "text_sh"+(parseInt(i));
				document.getElementById(text_arr).value = jsonfile.text_sh[i];
			}	
			for (i in jsonfile.but){
				but[i] = jsonfile.but[i];
			}
			for (i in jsonfile.mes){
				mes[i] = jsonfile.mes[i];
			}
			restartMessage = jsonfile.text_h[20]
			document.getElementById("text18").value =jsonfile.text_h[18];
			document.getElementById("Button1").value = but[4];
			document.getElementById("Button2").value = but[5];
			document.getElementById("Button3").value = but[6];
			document.getElementById("Button4").value = but[7];			
			document.getElementById("Button5").value = but[7];	
			document.getElementById("Button6").value = but[7];
			document.getElementById("Button7").value = but[7];
			document.getElementById("Button8").innerHTML =but[8];
			document.getElementById("Button9").value = but[9];	
			document.getElementById("Button10").value = but[10];	
		}
	};
	xhttp.send();	
}

function openValues(){
	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function() {
		if (this.readyState == 4 && this.status == 200) {
			var configSetup = JSON.parse(this.responseText);
			var selectList = document.getElementById("select");
			for (i in configSetup.lang){
				var option = document.createElement("option");
				option.value = configSetup.lang[i];
				option.text = configSetup.lang[i];
				selectList.appendChild(option);
			}
			document.getElementById("input1").value = configSetup.date_man;
			document.getElementById("input2").value = configSetup.time_man;
			document.getElementById("input3").value = configSetup.timeZone;
			document.getElementById("input4").value = configSetup.time_vkl[1];
			document.getElementById("input5").value = configSetup.time_rassvet[1];
			document.getElementById("input6").value = configSetup.time_zakat[1];			
			document.getElementById("input7").value = configSetup.time_otkl[1];
			document.getElementById("input8").value = configSetup.time_vkl[2];			
			document.getElementById("input9").value = configSetup.time_rassvet[2];			
			document.getElementById("input10").value = configSetup.time_zakat[2];
			document.getElementById("input11").value = configSetup.time_otkl[2];			
			document.getElementById("input12").value = configSetup.time_vkl[3];
			document.getElementById("input13").value = configSetup.time_rassvet[3];			
			document.getElementById("input14").value = configSetup.time_zakat[3];			
			document.getElementById("input15").value = configSetup.time_otkl[3];
			document.getElementById("input16").value = configSetup.time_vkl[4];
			document.getElementById("input17").value = configSetup.time_rassvet[4];			
			document.getElementById("input18").value = configSetup.time_zakat[4];			
			document.getElementById("input19").value = configSetup.time_otkl[4];
			document.getElementById("input20").value = configSetup.time_vkl[5];
			document.getElementById("input21").value = configSetup.time_rassvet[5];			
			document.getElementById("input22").value = configSetup.time_zakat[5];
			document.getElementById("input23").value = configSetup.time_otkl[5];			
			document.getElementById("input24").value = configSetup.time_vkl[6];
			document.getElementById("input25").value = configSetup.time_rassvet[6];			
			document.getElementById("input26").value = configSetup.time_zakat[6];
			document.getElementById("input27").value = configSetup.time_otkl[6];			
			document.getElementById("input28").value = configSetup.time_vkl[0];
			document.getElementById("input29").value = configSetup.time_rassvet[0];
			document.getElementById("input30").value = configSetup.time_zakat[0];
			document.getElementById("input31").value = configSetup.time_otkl[0];
			document.getElementById("input32").value = configSetup.temp_koef;
			document.getElementById("input33").value = configSetup.max_day;
			document.getElementById("input34").value = configSetup.max_night;
			document.getElementById("input35").value = configSetup.fan_start;
			document.getElementById("input36").value = configSetup.fan_stop;
			document.getElementById("input37").value = configSetup.ten_start;
			document.getElementById("input38").value = configSetup.ten_stop;
			document.getElementById("input39").value = configSetup.ssidAP;
			document.getElementById("input40").value = configSetup.passwordAP;
			document.getElementById("input41").value = configSetup.ssid;
			document.getElementById("input42").value = configSetup.password;
			document.getElementById("text_t1").innerText='ver: ' + configSetup.ver;
			defaultLang = configSetup.defaultLang;	
			selectList.value = defaultLang;
			openlang(defaultLang);
		}
	};
xhttp.open("GET", "configSetup.json", true);
xhttp.send();
}

function ButtonClick(buttonNumb, url, setText) {
	var message = new Object();	
	message.loading = but[0];
	message.success = but[1];
	message.failure = but[2];
	message.complete = but[3];
	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function() {
		if (this.readyState == 4 && this.status == 200) {
			console.log(message.success);	
			buttonNumb.value = message.success;
			buttonNumb.setAttribute("class", "btn btn-success");	
			setTimeout(() => { 
				console.log(message.complete); 
				buttonNumb.value = setText;
				buttonNumb.setAttribute("class", "btn btn-primary");			
			}, 1000);
		} else if (this.readyState < 4){
			console.log(message.loading);
			buttonNumb.value = message.loading;
			buttonNumb.setAttribute("class", "btn btn-warning");	
		} else {
			console.log(message.failure);		   
			buttonNumb.value = message.failure;
			buttonNumb.setAttribute("class", "btn btn-danger");	
			setTimeout(() => { 
				console.log(message.complete); 
				buttonNumb.setAttribute("class", "btn btn-primary");
				buttonNumb.value = setText;
			}, 1000);
		}
	};
	xhttp.open("GET", url, true);
	xhttp.send();
/* 	openValues(); */
}

function init(){
	var button1 = document.getElementById("Button1");
	var button2 = document.getElementById("Button2");
	var button3 = document.getElementById("Button3");
	var button4 = document.getElementById("Button4");
	var button5 = document.getElementById("Button5");
	var button6 = document.getElementById("Button6");   
	var button7 = document.getElementById("Button7");
	button1.addEventListener("click", function(event){ 
		event.preventDefault(); 
		var in1 = document.getElementById("input1").value;
		var in2 = document.getElementById("input2").value;
		var jsonData = 'date_man=' + in1 + '&time_man=' + in2;
		//return false;
		ButtonClick(button1, 'save_date?'+jsonData, but[4]); 
	});
	button2.addEventListener("click", function(event){ 
		event.preventDefault(); 
		var in3 = document.getElementById("input3").value;
		var jsonData = 'timeZone=' + in3;
		ButtonClick(button2, 'auto_sync?'+jsonData, but[5]); 
	});	
	button3.addEventListener("click", function(event){ 
		event.preventDefault(); 
		var in4  = document.getElementById("input4").value;
		var in5  = document.getElementById("input5").value;
		var in6  = document.getElementById("input6").value;
		var in7 =  document.getElementById("input7").value;
		var in8 =  document.getElementById("input8").value;
		var in9 =  document.getElementById("input9").value;
		var in10 = document.getElementById("input10").value;
		var in11 = document.getElementById("input11").value;
		var in12 = document.getElementById("input12").value;
		var in13 = document.getElementById("input13").value;
		var in14 = document.getElementById("input14").value;
		var in15 = document.getElementById("input15").value;
		var in16 = document.getElementById("input16").value;
		var in17 = document.getElementById("input17").value; 
		var in18 = document.getElementById("input18").value;
		var in19 = document.getElementById("input19").value;
		var in20 = document.getElementById("input20").value;
		var in21 = document.getElementById("input21").value;
		var in22 = document.getElementById("input22").value;
		var in23 = document.getElementById("input23").value;
		var in24 = document.getElementById("input24").value;
		var in25 = document.getElementById("input25").value;
		var in26 = document.getElementById("input26").value;
		var in27 = document.getElementById("input27").value;
		var in28 = document.getElementById("input28").value;
		var in29 = document.getElementById("input29").value;
		var in30 = document.getElementById("input30").value;
		var in31 = document.getElementById("input31").value;
		var jsonData = 'time_vkl_1='+in4+'&time_vkl_2='+in8+'&time_vkl_3='+in12+'&time_vkl_4='+in16+'&time_vkl_5='+in20+'&time_vkl_6='+in24+'&time_vkl_0='+in28+
                      '&time_rassvet_1='+in5+'&time_rassvet_2='+in9+'&time_rassvet_3='+in13+'&time_rassvet_4='+in17+'&time_rassvet_5='+in21+'&time_rassvet_6='+in25+'&time_rassvet_0='+in29+
					  '&time_zakat_1='+in6+'&time_zakat_2='+in10+'&time_zakat_3='+in14+'&time_zakat_4='+in18+'&time_zakat_5='+in22+'&time_zakat_6='+in26+'&time_zakat_0='+in30+
					  '&time_otkl_1='+in7+'&time_otkl_2='+in11+'&time_otkl_3='+in15+'&time_otkl_4='+in19+'&time_otkl_5='+in23+'&time_otkl_6='+in27+'&time_otkl_0='+in31;
		ButtonClick(button3, 'save_schedule?'+jsonData, but[6]); 
	});
	button4.addEventListener("click", function(event){ 
		event.preventDefault(); 
		var in32 = document.getElementById("input32").value;
		var in33 = document.getElementById("input33").value;
		var in34 = document.getElementById("input34").value;
		var in35 = document.getElementById("input35").value;
		var in36 = document.getElementById("input36").value;
		var in37 = document.getElementById("input37").value;
		var in38 = document.getElementById("input38").value;
		var jsonData = 'temp_koef='+ in32 +'&max_day='+ in33 +'&max_night='+ in34 +
            '&fan_start='+ in35 +'&fan_stop='+ in36 +'&ten_start='+ in37 +'&ten_stop='+ in38;
		ButtonClick(button4, 'save?'+jsonData, but[7]); 
	});	
	button5.addEventListener("click", function(event){ 
		event.preventDefault(); 
		var in39 = document.getElementById("input39").value;
		var in40 = document.getElementById("input40").value;
		var jsonData = 'ssidAP='+ in39 +'&passwordAP='+ in40;
		ButtonClick(button5, 'ssidAP?'+jsonData, but[7]); 
	});		
	button6.addEventListener("click", function(event){ 
		event.preventDefault(); 
		var in41 = document.getElementById("input41").value;
		var in42 = document.getElementById("input42").value;
		var jsonData = 'ssid='+ in41 +'&password='+ in42;
		ButtonClick(button6, 'ssid?'+jsonData, but[7]); 
	});		
	button7.addEventListener("click", function(event){ 
		event.preventDefault(); 
		var in43 = document.getElementById("input43").value;
		var jsonData = 'portNumber='+ in43;
		ButtonClick(button7, 'portNumber?'+jsonData, but[7]); 
	});	
}

function liveValues() {
    ws = new WebSocket("ws://" + window.location.hostname+":49202/");
    ws.onopen = function(event) {
		// var enableMessage = JSON.stringify({enableGestures: true});
		console.log("[open] Websocket-соединение установлено...");
    };
    ws.onmessage = function(event) {
		//console.log("[message] Данные с сервера получены");
		//console.log(event.data);
		var json = JSON.parse(event.data);
		var parsed_day, parsed_month, parsed_year, parsed_hour, parsed_min;
		parsed_day = json.now.substring(0, 2);
		parsed_month = json.now.substring(3, 5);
		parsed_year = json.now.substring(6, 10);
		parsed_hour = json.now.substring(13, 15);
		parsed_min = json.now.substring(16, 18);		
        if (parsed_day < 1 || parsed_day > 31 || parsed_month < 1 || parsed_month > 12 || parsed_hour < 0 || parsed_hour > 24 || parsed_min < 0 || parsed_min > 60){
            if (parsed_day == 165 || parsed_month == 165 || parsed_hour == 165 || parsed_min == 85) {
				document.getElementById("text_t3").innerHTML = mes[1];
				document.getElementById("text_t3").style="color:red;width:40%;vertical-align:middle;display:table-cell;";
				console.log(json.now);
			} else {
				document.getElementById("text_t3").innerHTML = mes[2];
				document.getElementById("text_t3").style="color:red;width:40%;vertical-align:middle;display:table-cell;";
				console.log(json.now);
			} 
		} else {
			document.getElementById("text_t3").innerHTML = json.now;
            document.getElementById("text_t3").style="color:black;width:40%;vertical-align:middle;display:table-cell;";
		}
		if       (json.rssi >= -50) 					  { class_wifi = 'wifi4'; } 
		else if ((json.rssi >= -70) && (json.rssi < -50)) { class_wifi = 'wifi3'; } 
		else if ((json.rssi >= -85) && (json.rssi < -70)) {	class_wifi = 'wifi2'; } 
		else if ((json.rssi >= -110) && (json.rssi < -85)){ class_wifi = 'wifi1'; }
		if (json.led == 0) { class_led = 'led_0'; } 
		else 			   { class_led = 'led_1'; }
		class_fan = 'fan_' + json.fan;
		class_ten = 'ten_' + json.ten;
        document.getElementById("Picture2").setAttribute("class", class_wifi);
        document.getElementById("Picture3").setAttribute("class", class_led);
        document.getElementById("Picture4").setAttribute("class", class_fan);        
		document.getElementById("Picture5").setAttribute("class", class_ten);	
		document.getElementById("text_t5").innerHTML = json.led+"%";		
		document.getElementById("text_t4").innerHTML = json.rssi+" dBm";	
		if (json.temp == 0.00){
			document.getElementById("text_t2").innerHTML = mes[0];
            document.getElementById("text_t2").style="color:red;text-align:left;vertical-align:middle;display:table-cell;";			
		    document.getElementById("Picture1").setAttribute("class", "tempna");	
		}
		else{
			document.getElementById("text_t2").innerHTML = json.temp + " °C";
            document.getElementById("text_t2").style="color:black;text-align:left;vertical-align:middle;display:table-cell;";			
		    document.getElementById("Picture1").setAttribute("class", "temp");
		}
		if (json.graph_changing == 1){
			ws.send(0);	
			updateChart();
		}
	};
    ws.onclose = function(event) {
		ws = null;
		console.log("[close] Websocket-соединение разорвано...");
    };
    ws.onerror = function(event) {
		console.log("[error] Ошибка подключения...");
    };
}
	  
function updateChart() {
	var xhttp = new XMLHttpRequest();
	xhttp.open("GET", "configChart.json", true);
	xhttp.onreadystatechange = function() {
		if (this.readyState == 4 && this.status == 200) {
			var jsonfile = JSON.parse(this.responseText);
			var chart = new Chartist.Line('.ct-chart', {
			  labels: jsonfile.date,
			  series: [jsonfile.values_filtered]
			}, {
			  fullWidth: true,
			  width: '500px',
			  height: '400px' 		  
			});
		}
	};
	xhttp.send();
}

function add_zero(fromJson){
    var value_man;
    if (fromJson < 10){
      value_man = "0" + parseInt(fromJson, 10);
    }
    else{
      value_man = fromJson;
    }
    return value_man;
  }