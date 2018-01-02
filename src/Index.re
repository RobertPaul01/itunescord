type activityData = {. "details": string, "largeImageKey": string, "state": string};

type clientData = {. "transport": string};

type songData = {. "artist": string, "album": string, "name": string};

type rpcObj = {
  .
  [@bs.meth] login : string => unit,
  [@bs.meth] on : (string, unit => unit) => unit,
  [@bs.meth] setActivity : activityData => unit
};

[@bs.module "discord-rpc"] [@bs.new] external client : clientData => Js.t(rpcObj) = "Client";

[@bs.module "playback"] external onPlayback : (string, songData => unit) => unit = "on";

let appClient = "395127794029428736";

let appImage = "itunes_logo_large";

let rpc = client({"transport": "ipc"});

rpc##login(appClient);

let onReady = () => {
  onPlayback(
    "playing",
    (data) => {
      let song = data##name;
      let artist = data##artist;
      rpc##setActivity({
        "details": {j|🎵 Listening to|j},
        "state": song ++ " by " ++ artist,
        "largeImageKey": appImage
      })
    }
  );
  onPlayback(
    "paused",
    (data) => {
      let song = data##name;
      let artist = data##artist;
      rpc##setActivity({
        "details": "Paused...",
        "state": song ++ " by " ++ artist,
        "largeImageKey": appImage
      })
    }
  )
};

rpc##on("ready", onReady);