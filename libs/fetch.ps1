$client = new-object System.Net.WebClient
$path = $(get-location).tostring()
Write-Host($path)
$urls = @("http://www.libsdl.org/release/SDL2-devel-2.0.1-VC.zip" 
    "http://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-devel-2.0.0-VC.zip"
    "http://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.0.12-VC.zip")
foreach ($url in $urls) {
    Write-Host($url)
    Write-Host($path)
    $client.DownloadFile( $url, $path + "\\" + $url.SubString($url.LastIndexOf("/")+1));
}
