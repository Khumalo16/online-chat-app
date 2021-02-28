const allchannels = localStorage.getItem('rooms');
let rooms; 
document.addEventListener('DOMContentLoaded', function() {

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

    function message(channel) {
        var socket = io.connect(location.protocol + '//' + document.domain + ':' + location.port);
        socket.on('connect', () => {
            document.querySelectorAll('#button').forEach(button => {
                button.onclick = ()=> {
                    var msg = document.getElementById('text').value;
                    socket.emit('message sent', {'msg': msg});
                };
            })
           
        });

        socket.on('message back', data => {
      
        const li = document.createElement('li');
       
        li.innerHTML = data.msg;
        document.querySelector(`.${channel}`).append(li);
          
        });
    }   
    
    message(localStorage.getItem('channelClicked'));

    function showChannel(channel) {
       
      if (document.querySelector(`.${channel}`).style.display === 'none') {
        document.querySelector(`.${channel}`).style.display = 'block';

      }  else {
        document.querySelector(`.${channel}`).style.display = 'none';

      }
       
    }

    document.querySelectorAll('#channel').forEach(channel => {
      
        channel.onclick = function() {
            localStorage.setItem('channelClicked',this.dataset.channel);
            showChannel(this.dataset.channel);

        } ;
      });
   
       
     
      function create() {
         
          var socket = io.connect(location.protocol + '//' + document.domain + ':' + location.port);
          socket.on('connect', () => {
            document.getElementById('channelsub').onclick = function() {
                var c = document.getElementById('nameChannel').value;

                socket.emit('channel name', {'li': c});
            }
            
          });
  
          socket.on('channel name', data => {
            const li = document.createElement('li');
            const a = document.createElement('a');
            const ul = document.createElement('ul');
    
            a.href = "";
            a.id = "all";
            a.dataset.channel = data.li;
            a.innerHTML = data.li;
    
            ul.id = "channel";
            ul.className = data.li;
           
            li.insertAdjacentElement('afterbegin',a);
            li.insertAdjacentElement('afterend',ul);
            localStorage.setItem(`${data.li}`,li);
            
            if ( allchannels === null) {
                rooms = [];
            } else {
                rooms = JSON.parse(allchannels);
            }
            rooms.push(li);
            console.log(rooms);
            localStorage.setItem('rooms', JSON.stringify(rooms))
            document.getElementById('addchannel').append(li);
            document.querySelector('.create').style.display= 'none';

          });


    }

    create();
          
});


