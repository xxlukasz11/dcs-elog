
let lis = document.getElementById('menu').getElementsByTagName('ul')[0].getElementsByTagName('li');
let tabs = document.getElementsByClassName('tab');

for(let i = 0; i < lis.length; ++i){
	lis[i].addEventListener('click', function(){
		openTab(lis[i]);
	}, false);
}

function openTab(target){
	// hide all tabs
	for(let t of tabs){
		t.style.display = 'none';
	}
	
	let id = target.getAttribute('id') + '_tab';
	document.getElementById(id).style.display = 'block';
	
	// change active tab in menu
	for(let i = 0; i < lis.length; ++i){
		lis[i].className = lis[i].className.replace(" active", "");
	}
	target.className += " active";
}