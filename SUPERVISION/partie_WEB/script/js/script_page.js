function init_page(){
	//Positionnement du nav:
	$('nav').css('top',$('header').height());
	$('#display_graph').hide();

	//Adaptation aux dimentions de la fenettre:
	$("#all_ip").height($(window).height());

	//Positionnement du menu haut:
}
function actualise(){
	$('#all_ip').load("script/php/recup_info.php");
}

actualise();
setInterval(function(){
    actualise(); // this will run after every 5 seconds
}, 20000);

init_page();


//Differentes actions sur le boutons:
var bascule_bouton_menu=0;
$('#bouton_menu').click(function(){
	bascule_bouton_menu=1-bascule_bouton_menu;
	if(bascule_bouton_menu==1){
		$('nav').hide();
		$(this).css('background','white').css('color','black');
		//On colle les ip à gauche:
		$('#all_ip').css('padding-left','0px');


	}
	else{
		$('nav').show();
		$(this).css('background','red').css('color','white');
		$('#all_ip').css('padding-left',$('nav').width());
	}
});
var bascule_parametre=0;
$('#parametre').click(function(){
	bascule_parametre=1-bascule_parametre;
	if(bascule_parametre==1){
		//On cache les anciennes balises	
		$('nav').hide();
		$('#all_ip').hide();
		$('.menu_haut').each(function(){
			$(this).hide();
		});
		$('#parametre_div').css('visibility','Visible');
	}
	else{
		$('nav').show();
		$('#all_ip').show();
		$('.menu_haut').each(function(){
			$(this).show();
		});
		$('#parametre_div').css('visibility','hidden');
		var tmp_max_ping=$("#param_temps_max_ping").val();
		var tmp_sms=$("#param_temps_avant_sms").val();
		var tmp_rafraichissement_ping=$("#param_temps_ping").val();
		$.get('set_param.php/?tmp_max_ping='+tmp_max_ping+"&tmp_sms="+tmp_sms+"&tmp_rafraichissement_ping="+tmp_rafraichissement_ping);
		alert($("#param_file").val());
	

	}
	



});
$(document).on('click', '.button',function () {
	/*
		Fonction permettant de recuperer les differentes options d'administration associee à  une ip 
	*/
	//Better to construct options first and then pass it as a parameter
	var options = {
		title: {
			text: "Spline Chart using jQuery Plugin"
		},
                animationEnabled: true,
		data: [
		{
			type: "spline", //change it to line, area, column, pie, etc
			dataPoints: [
				{ x: new Date(2017,5,5,16,05), y: 10 },
				{ x: new Date(2017,5,5,17,10), y: 12 },
				{ x: new Date(2017,5,5,18,25), y: 8 },
				{ x: new Date(2017,5,5,19,30), y: 14 }
			]
		}
		]
	};

	//On cache les IP
	$('#all_ip').hide();



	$("#chartContainer").CanvasJSChart(options);
	});

