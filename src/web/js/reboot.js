const rebootbutton = document.querySelector('#rebootbutton')
const confirmrebootform = document.querySelector('.confirmrebootform')
const cancelrebootbutton = document.querySelector('#cancelreboot')
const confirmrebootbutton = document.querySelector('#confirmreboot')
const loader = document.querySelector('.loader')

rebootbutton.addEventListener('click', () => {
    confirmrebootform.classList.add('active')
})

const reboot = () => {
    loader.classList.add('active')
    fetch(routes.reboot(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem("token")}"}`
    }).then(data => data.json()).then(jsondata => {

    }).catch((error) => {
        // notification(`Перезагрузка`, "success")
        confirmrebootform.classList.remove('active')
        // window.location.replace(window.location.origin)
        loader.classList.add('active')
        waitreboot()
    })
}

cancelrebootbutton.addEventListener('click', () => {
    confirmrebootform.classList.remove('active')
})

confirmrebootbutton.addEventListener('click', reboot)

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
