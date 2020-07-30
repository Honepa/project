var id_button = {
    forward : 0,
    back    : 0,
    right   : 0,
    left    : 0,
    on_line : 0,
    stop    : 0,
    speed   : 0,
    };
let timer_id = setInterval(fl_drive, 500);
function fl_drive(){
    $.ajax({
        type: "POST",
        url: "/",
        data: id_button,
        
    });
};    
$(window).on('mouseup', function(){
    id_button.forward = 0;
    alert(id_button.forward);
});

document.getElementById("forward").addEventListener("mousedown", function(){
    id_button.forward = 1;
    id_button.back    = 0;
    id_button.left    = 0;
    id_button.right   = 0;
    id_button.on_line = 0;
    id_button.stop    = 0;
    
});
document.getElementById("back").addEventListener("mousedown", function(){
    id_button.forward = 0;
    id_button.back    = 1;
    id_button.left    = 0;
    id_button.right   = 0;
    id_button.on_line = 0;
    id_button.stop    = 0;
    alert("back");
});
document.getElementById("left").addEventListener("mousedown", function(){
    id_button.forward = 0;
    id_button.back    = 0;
    id_button.left    = 1;
    id_button.right   = 0;
    id_button.on_line = 0;
    id_button.stop    = 0;
    alert("left");
});
document.getElementById("right").addEventListener("mousedown", function(){
    id_button.forward = 0;
    id_button.back    = 0;
    id_button.left    = 0;
    id_button.right   = 1;
    id_button.on_line = 0;
    id_button.stop    = 0;
    alert("right");
});
document.getElementById("on_line").addEventListener("mousedown", function(){
    id_button.forward = 0;
    id_button.back    = 0;
    id_button.left    = 0;
    id_button.right   = 0;
    id_button.on_line = 1;
    id_button.stop    = 0;
    alert("line");
});
document.getElementById("stop").addEventListener("mousedown", function(){
    id_button.speed = 0;
    id_button.stop    = 1;
    alert("stop");
});
      
document.getElementById("speed").addEventListener("click", function() {
    id_button.speed=this.value;
    this.value = id_button.speed;
    alert(id_button.speed);
});