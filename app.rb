require 'sinatra'
require 'sinatra/reloader' # この行を追加。sinatra-contribはこのために必要


get '/img/:fn' do |fn|
  send_file(fn)
end

get '/ajax' do |*e|
  puts "="*10
  p [ e, params ]
  cx = params["x"].to_f
  cy = params["y"].to_f
  w = params["w"].to_f
  d = params["d"].to_f
  wpix = params["wpix"].to_f
  %x(./Main #{wpix} #{d} #{cx} #{cy} #{w} )
  ""
end

get '/' do
  <<~HTML
  <style>
  img{ width:calc(100vw - 160px) }
  input.float{ width:15ex }
  input.int{ width:10ex }
  .sidebar {
    width: 140px;
  }
  .main {
    width: calc(100vw - 150px);
  }
  .root {
    width: calc(100vw - 20px);
    margin: 0 auto;
    display: flex;
    justify-content: space-between;
  }
  </style>
  <div class="root">
    <aside class="sidebar">
      <label for="cx">cx:</label>
      <input class="float" id="cx" type="text"> <br/>
      <label for="cy">cy:</label>
      <input class="float" id="cy" type="text"> <br/>
      <label for="w">w:</label>
      <input class="float" id="w" type="text"> <br/>
      <button id="zout">zoom out</button>
      <br/>
      <label for="wpix">wpix:</label>
      <input class="int" id="wpix" type="text"> <br/>
      <label for="d">d:</label>
      <input class="int" id="d" type="text"> <br/>
    </aside>
    <div class="main">
      <img id="img" src="img/hoge.png"/>
    </div>
  </div>
  <script>
    const set = ( name, val )=>{
      document.getElementById(name).value = val;
    };
    const getnum = ( name )=>{
      return parseFloat(document.getElementById(name).value);
    };

    const newCenter = (rel, c, w0, w1 )=>{
      const left0 = c-w0/2;
      const fix = left0 + w0*rel;
      const left1 = fix - w1*rel;
      return left1 + w1/2
    };

    const update = ()=>{
      const w  = getnum("w");
      const cx = getnum("cx");
      const cy = getnum("cy");
      const wpix = getnum("wpix");
      const depth = getnum("d");
      const xhr = new XMLHttpRequest();
      xhr.open('GET', `/ajax?wpix=${wpix}&d=${depth}&x=${cx}&y=${cy}&w=${w}`)
      xhr.onreadystatechange = () => {
        if(xhr.readyState === XMLHttpRequest.DONE) {
          var status = xhr.status;
          if (status === 0 || (status >= 200 && status < 400)) {
            img.src = "img/hoge.png?x="+Date.now();
            console.log(xhr.responseText);
          } else {
            // console.log("Something wrong:", xhr );
          }
        }
      };
      xhr.send(null);
    };

    document.getElementById("zout").onclick = ()=>{
      set("w", getnum("w")*2);
      update();
    };

    const img = document.getElementById("img");
    img.onclick = (e)=>{
      const relX = e.offsetX / img.width;
      const relY = e.offsetY / img.height;
      console.log(relX, relY);
      const w0  = getnum("w");
      const w = w0*0.75;
      const cx0 = getnum("cx");
      const cy0 = getnum("cy");
      const cx = newCenter( relX, cx0, w0, w );
      const cy = newCenter( relY, cy0, w0, w );
      set( "cx", cx );
      set( "cy", cy );
      set( "w", w);
      console.log( [cx0,cy0,w0], "=>", [cx,cy,w] )
      update();
    };
    const init = ()=>{
      set( "cx", -0.5 );
      set( "cy", -0.5 );
      set( "w", 3.5 );
      set( "wpix", 800 );
      set( "d", 200 );
    };
    init();
  </script>
  HTML
end
