
document.addEventListener('DOMContentLoaded', () => {

    // Set local storage
    

    localStorage.clear();
    if (localStorage.getItem("username") === null) {
        //...
        load_page(registration.dataset.registration);
     
        console.log("trying")
            function load_page(name) {
            const request = new XMLHttpRequest();
            request.open('GET',`/${name}`);
            request.onload = () => {
                const response = request.responseText;
                document.getElementById('container').innerHTML= response;
            };
            request.send();
        }
    }
  
    var name = document.querySelector('.fo').value;
    console.log(name);
   
    // Connect to websocket
  
 
  /*  var socket = io.connect(location.protocol + '//' + document.domain + ':' + location.port);

    // When connected, configure buttons
    socket.on('connect', () => {

        // Each button should emit a "submit vote" event
        document.querySelectorAll('button').forEach(button => {
            button.onclick = () => {
                const selection = document.getElementById('text').value;
                socket.emit('submit vote', {'selection': selection});
            };
        });
    });

    // When a new vote is announced, add to the unordered list
    socket.on('announce vote', data => {
        const li = document.createElement('li');
        li.innerHTML = `${data.selection}`;

        document.querySelector('#all').append(li);
    }); */
});

 