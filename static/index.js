
document.addEventListener('DOMContentLoaded', () => {

   //localStorage.clear();


    if (localStorage.getItem("username") === null) {
        //...
        load_page(registration.dataset.registration);
        document.addEventListener('click', e => {
                if(e.target && e.target.id == 'btn-primary') {
                    const user = document.querySelector('.form-control').value;
                    localStorage.setItem('user', user );
                }
        });
    } 
   
   if (localStorage.getItem('user') != null && localStorage.getItem('username') === null) {
        var socket = io.connect(location.protocol + '//' + document.domain + ':' + location.port);
        socket.on('connect', () => {
            const user = localStorage.getItem("user");
            socket.emit('username', {'user': user});
        });

        socket.on('username', data => {
            localStorage.setItem("username", data.user);    
        });

        load_page("");
        function refresh() {    
            setTimeout(function () {
                location.reload()
            }, 100);
        }
       refresh();
   }

    function load_page(name) {
        const request = new XMLHttpRequest();
        request.open('GET',`/${name}`);
        request.onload = () => {
            const response = request.responseText;
            document.getElementById('container').innerHTML= response;
        };
        request.send();
    }

    function message() {
        var socket = io.connect(location.protocol + '//' + document.domain + ':' + location.port);
        socket.on('connect', () => {
            document.querySelectorAll('button').forEach(button => {
                button.onclick = ()=> {
                    socket.emit('message sent');
                };
            })
           
        });

        socket.on('message back', () => {
      
        const li = document.createElement('li');
        li.innerHTML = document.getElementById('text').value;
        document.getElementById('all').append(li);
          
        });
    }   
    
    message();
});


