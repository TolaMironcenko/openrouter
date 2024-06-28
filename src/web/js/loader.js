const loader = document.querySelector('.loader')
const loaderheader = document.querySelector('#loaderheader')

const defaultloaderheader = ""

const start_loader = (header = defaultloaderheader) => {
    loaderheader.innerHTML = header
    loader.classList.add('active')
}

const stop_loader = () => {
    loader.classList.remove('active')
}