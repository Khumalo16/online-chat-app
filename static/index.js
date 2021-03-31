const allChannel = localStorage.getItem('names');
let names;


document.addEventListener('DOMContentLoaded', function() {

  //localStorage.clear();
   

       showAllChannels();

    if (localStorage.getItem("username") === null) {
        //...
        load_page("registration");
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

      function load_sms(channel, name) {
        const request = new XMLHttpRequest();
        request.open('GET',`/${name}`);
        request.onload = () => {
            const response = request.responseText;
            document.getElementById('container').innerHTML= response;
            let indivisual = localStorage.getItem(`${channel}`);
            names = JSON.parse(indivisual);

            showAllChannels();

            for ( var i = 0; i < indivisual.length; i++) {
            
                let li = document.createElement('li');
                li.innerHTML= indivisual[i];
                document.getElementById('sms').append(li);
            }
    

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
        const singSMS = localStorage.getItem(`${channel}`);
        let sms;
        if (singSMS === null) {
            sms = [];
        } else {
            sms = JSON.parse(singSMS);
        }
            
            sms.push(data.msg);
            localStorage.setItem(`${channel}`, JSON.stringify(sms));
          
        });
    }   
    
    message(localStorage.getItem('channelClicked'));

    function showChannel(channel) {
       
      if (document.querySelector(`.${channel}`).style.display === 'block') {
        document.querySelector(`.${channel}`).style.display = 'none';

      }  else {
        document.querySelector(`.${channel}`).style.display = 'block';

      }
       
    }

    document.querySelectorAll('#channel').forEach(channel => {
        document.querySelectorAll('#all').forEach(all => {
          //  all.style.display = 'none';
          });
        channel.onclick = function() {
            localStorage.setItem('channelClicked',this.dataset.channel);
       // showChannel(this.dataset.channel);
          
         //   load_sms(this.dataset.channel, "");
           // return false;
        } ;
      });
   
       
     
      function create() {
         
          var socket = io.connect(location.protocol + '//' + document.domain + ':' + location.port);
          socket.on('connect', () => {
            document.getElementById('channelsub').onclick = function() {
                var c = document.getElementById('nameChannel').value;
                var f = JSON.parse(allChannel);
                socket.emit('channel name', {'li': c});
            }
            
          });
  
          socket.on('channel name', data => {

            if (allChannel === null) {
                names = [];
            } else {
                names = JSON.parse(allChannel);
            }
            
            names.push(data.li);
            localStorage.setItem('names', JSON.stringify(names));
            console.log(localStorage.getItem('names'))
            
            let li = document.createElement('li');
                let a = document.createElement('a');
                let ul = document.createElement('ul');
                a.href = "";
                ul.id = "channel"
                a.dataset.channel = data.li;
                a.innerHTML = data.li;
        
                ul.id = "all"
                ul.className = data.li;
            
                li.append(a);
                document.getElementById('addChannelMsg').append(ul);
                document.getElementById('addChannel').append(li);
                document.querySelector('.create').style.display= 'none';

          });
    }

    function showAllChannels() {

            names = JSON.parse(allChannel);

            if (names != null) {
                   for ( var i = 0; i < names.length; i++) {
            
                let li = document.createElement('li');
                let a = document.createElement('a');
                let ul = document.createElement('ul');
                a.href = "";
                ul.id = "channel";
                a.id = "channel";
                a.dataset.channel = names[i];
                a.innerHTML = names[i];
        
                ul.id = "all"
                ul.className = names[i];
            
                li.append(a);
                li.append(ul);
                document.getElementById('addChannel').append(li);
            } 
            }
        
    
            
    }

    create();
    load_sms(localStorage.getItem('channelClicked'), "");
    let i = localStorage.getItem('1');
    if (i === null) {
        localStorage.setItem('1',1);
    } else {
        i = 1 + i;
        localStorage.setItem('1',i);
    }
    console.log(i);

    

});