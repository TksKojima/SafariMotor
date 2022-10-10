// Todo  R"=====( )=====""
const char inputPad_html[] PROGMEM = R"=====(
<!DOCTYPE html> 
<html lang='ja'> 
<head> 
<meta charset='UTF-8'/> 
<meta name='viewport' content='width=320, initial-scale=1.0, user-scalable=no'> 
<title>Car Controller</title> 
<style type='text/css'><!-- 
.btn_LX { width:124px; height: 36px; text-align:center; padding:8px; border-radius:7px; background-color:#CCCCCC; } 
--> 
</style> 
</head> 
<body> 
 
<section>
<CENTER>
　　X座標：<input type='text' id='txtX' />
　　Y座標：<input type='text' id='txtY' />
　　Steer%：<input type='text' id='Steer' />
　　Forward%：<input type='text' id='Forward' />

<div id='relative'>
<canvas id='drag_area' />  
</CENTER>
</div>

</section>

<script type='text/javascript'> 
  var canvas = document.getElementById('drag_area');

  window.onload = function() {
    var ctx = canvas.getContext('2d');

    function fitCanvasSize() {
      canvas.width  = document.documentElement.clientWidth*0.9;
      canvas.height = document.documentElement.clientHeight*0.9;
      
      
      if( canvas.width < canvas.height ){
        canvas.height = canvas.width ;
      }else{
        canvas.width = canvas.height ;
      }

  
      ctx.lineWidth = 10;
      linew = canvas.width*0.05;

      // 角丸四角形 
      ctx.fillStyle = 'rgb(100, 100, 100)';
      fillRoundRect(ctx, 0, 0, canvas.width, canvas.height, canvas.width*0.06);
      ctx.fillStyle = 'rgb(200, 200, 200)';
      fillRoundRect(ctx, linew/2, linew/2, canvas.width-linew, canvas.height-linew, canvas.width*0.05);
      
      var w = canvas.width;
      var h = canvas.height;
      var per5 = w * 0.05;
      ctx.beginPath();
      ctx.moveTo(0+per5,h/2);
      ctx.lineTo(w/2-per5,h/2);
      ctx.moveTo(w/2+per5,h/2);
      ctx.lineTo(w-per5,h/2);

      ctx.moveTo(w/2,0+per5);
      ctx.lineTo(w/2,h/2-per5);
      ctx.moveTo(w/2,h/2+per5);
      ctx.lineTo(w/2,h-per5);

      ctx.lineWidth = 0.5;
      ctx.stroke();
      
    }

    fitCanvasSize();
    window.onresize = fitCanvasSize;
  }


var elements = document.getElementsByClassName('drag-and-drop');
var x;
var y;

  var xhr = new XMLHttpRequest(), reqSend = 0, reqRet = 0; 
  xhr.onreadystatechange = HttpRes; 
  function HttpRes()  { 
    if (xhr.readyState == 4 && xhr.status == 200) reqRet = 1; 
  } 

  function HttpReq(v) { 
    xhr.open('GET', '/rc?' + v); 
    xhr.send(null); 
    reqRet = 0; 
  } 

  function nmn(n) { 
    if (n > -5 && n < 5) return(0); 
    if (n < -64) n = -64; 
    if (n >  64) n =  64; 
    return(n); 
  } 

  var vals = '', Fow = 0, St = 0; 
  var nowDrag = 0;

  function ratioCmp(n) { 

    var abs_n_wk0 = (Math.abs(n) - 5)*100/90;
    
    if( abs_n_wk0 < 0 ){
        abs_n_wk0 = 0;
    }
    if( abs_n_wk0 > 100 ){
        abs_n_wk0 = 100;
    }
    
    if( n < 0 ){
        return ( -1 * abs_n_wk0 );
    }else{
        return ( abs_n_wk0 );
    }
   
  }

/**
 * 角が丸い四角形のパスを作成する
 * @param  {CanvasRenderingContext2D} ctx コンテキスト
 * @param  {Number} x   左上隅のX座標
 * @param  {Number} y   左上隅のY座標
 * @param  {Number} w   幅
 * @param  {Number} h   高さ
 * @param  {Number} r   半径
 */
function createRoundRectPath(ctx, x, y, w, h, r) {
    ctx.beginPath();
    ctx.moveTo(x + r, y);
    ctx.lineTo(x + w - r, y);
    ctx.arc(x + w - r, y + r, r, Math.PI * (3/2), 0, false);
    ctx.lineTo(x + w, y + h - r);
    ctx.arc(x + w - r, y + h - r, r, 0, Math.PI * (1/2), false);
    ctx.lineTo(x + r, y + h);       
    ctx.arc(x + r, y + h - r, r, Math.PI * (1/2), Math.PI, false);
    ctx.lineTo(x, y + r);
    ctx.arc(x + r, y + r, r, Math.PI, Math.PI * (3/2), false);
    ctx.closePath();
}

/**
 * 角が丸い四角形を塗りつぶす
 * @param  {CanvasRenderingContext2D} ctx コンテキスト
 * @param  {Number} x   左上隅のX座標
 * @param  {Number} y   左上隅のY座標
 * @param  {Number} w   幅
 * @param  {Number} h   高さ
 * @param  {Number} r   半径
 */
function fillRoundRect(ctx, x, y, w, h, r) {
    createRoundRectPath(ctx, x, y, w, h, r);
    ctx.fill();
}

/**
 * 角が丸い四角形を描画
 * @param  {CanvasRenderingContext2D} ctx コンテキスト
 * @param  {Number} x   左上隅のX座標
 * @param  {Number} y   左上隅のY座標
 * @param  {Number} w   幅
 * @param  {Number} h   高さ
 * @param  {Number} r   半径
 */
function strokeRoundRect(ctx, x, y, w, h, r) {
    createRoundRectPath(ctx, x, y, w, h, r);
    ctx.stroke();       
}


function pt_move(e){

    if(e.type === 'mousemove') {
        var ev = e;
    } else {
        var ev = e.changedTouches[0];
    }
    
    if( nowDrag == 1){
  
    var mX = ev.pageX - canvas.offsetLeft; 
    var mY = ev.pageY - canvas.offsetTop; 
    
    var mX_ratio = (ev.pageX - canvas.offsetLeft)/(canvas.width); 
    var mY_ratio = (ev.pageY - canvas.offsetTop) /(canvas.height); 
    
    var mX_ratio100 = mX_ratio * 200 - 100;
    var mY_ratio100 = (mY_ratio * 200 - 100)*-1; 
    
    var mX_ratioCmd =  ratioCmp( mX_ratio100 );
    var mY_ratioCmd =  ratioCmp( mY_ratio100 );
    St  = mX_ratioCmd; 
    Fow = mY_ratioCmd;    
    document.getElementById('txtX').value = mX;
    document.getElementById('txtY').value = mY;
    
    document.getElementById('Steer').value   = Math.round( St );
    document.getElementById('Forward').value = Math.round( Fow );

    reqSend++; 
    }
    
}

function pt_up(){
      nowDrag = 0;
      St = 0;
      Fow = 0;
      document.getElementById('Steer').value   = Math.round( St );
      document.getElementById('Forward').value = Math.round( Fow );
}

function pt_down(){
      nowDrag = 1;  
      
      document.getElementById('Steer').value   = Math.round( St );
      document.getElementById('Forward').value = Math.round( Fow );    

}
  
  canvas.addEventListener('mousemove', (ev) => { 
    pt_move(ev);
    
  });   
  canvas.addEventListener('touchmove', (ev) => { 
    pt_move(ev);
    
  }); 
  
    window.addEventListener('mouseup', () => { 
      pt_up();
    });
    window.addEventListener('touchend', () => { 
      pt_up();
    });
    
    canvas.addEventListener('mousedown', () => { 
       pt_down();
    });    
    canvas.addEventListener('touchstart', () => { 
       pt_down();
    }); 

  var timer = window.setInterval(() => { 
    vals = ''; 
    //if (nowDrag > 0) { 
    vals = 'D0=' + Fow + '&D1=' + St ; 
    //} 
    HttpReq(vals); 
  }, 200); 

for(var i = 0; i < elements.length; i++) {
    elements[i].addEventListener('mousedown', mdown, false);
    elements[i].addEventListener('touchstart', mdown, false);
}
  
</script> 
</body> 
</html>
)=====";
