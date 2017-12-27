type activityData = {.
  "details": string, 
  "largeImageKey": string,
  "state": string,
};

type clientData = {.
    "transport": string
};

type rpcObj = Js.t({.
  [@bs.meth] login: string => unit,
  [@bs.meth] on: (string, unit => unit) => unit,
  [@bs.meth] setActivity: activityData => unit
});

type songData = Js.t({.
    artist: string,
    album: string,
    name: string
});

[@bs.module "discord-rpc"] [@bs.new] external client : clientData => rpcObj = "Client";
[@bs.module "playback"] external onPlayback : (string, songData => unit) => unit = "on"; 

let appClient = "395127794029428736";
let appImage = "itunes_logo_large";
let rpc = client({"transport": "ipc"});
rpc##login(appClient);

let onReady = () => {
    onPlayback("playing", data => {
        let song = data##name;
        let artist = data##artist;
        let album = data##album;
        rpc##setActivity({ 
            "details": {j|🎵 $song|j},
            "state": {j|👤 $artist - $album|j},
            "largeImageKey": appImage 
        });
    });
};

rpc##on("ready", onReady);
