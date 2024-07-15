const rebootbutton = document.querySelector('#rebootbutton')

rebootbutton.addEventListener('click', () => {
    if (confirm("Confirm reboot?")) {
        reboot()
    }
})

const reboot = () => {
    loader.classList.add('active')
    fetch(routes.reboot(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem("token")}"}`
    }).then(data => data.json()).then(jsondata => {

    }).catch((error) => {
        start_loader("Rebooting")
        setTimeout(waitreboot, 2000)
    })
}

const waitreboot = () => {
    fetch(routes.uptime()).then(res => res.json()).then(jsondata => {
        stop_loader()
    }).catch((error) => {
        waitreboot()
    })
}
