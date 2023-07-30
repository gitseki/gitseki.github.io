let tempReading = document.getElementById('temp')
let activityStatus = document.getElementById('activeorinactive')

function textchecker(){
  if (tempReading.innerHTML){
    activityStatus.innerHTML = 'ACTIVE'
  } else {
    activityStatus.innerHTML = 'LOADING...'
  }
  console.log(tempReading.innerHTML);
}

setInterval(textchecker, 3000);

textchecker();