const rebootbutton = document.querySelector('#rebootbutton')
const loader = document.querySelector('.loader')

rebootbutton.addEventListener('click', () => {
    get_confirm_form("Confirm reboot?", reboot)
})

const reboot = () => {
    loader.classList.add('active')
    fetch(routes.reboot(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem("token")}"}`
    }).then(data => data.json()).then(jsondata => {

    }).catch((error) => {
        loader.classList.add('active')
        setTimeout(waitreboot, 2000)
    })
}

const waitreboot = () => {
    fetch(routes.telnet_status(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem("token")}"}`
    }).then(res => res.json()).then(jsondata => {
        loader.classList.remove('active')
    }).catch((error) => {
        waitreboot()
    })
}
