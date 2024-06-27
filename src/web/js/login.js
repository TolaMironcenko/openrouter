const loginform = document.querySelector('.loginform')
const loginformbody = document.querySelector('.loginformbody')
const loginusername = document.querySelector("#loginusernameinput")
const loginpassword = document.querySelector("#loginpasswordinput")
const exitbutton = document.querySelector('.exitbutton')
const adminpanelsection = document.querySelector('.adminpanel')
const headerusername = document.querySelector('.headerusername')

if (localStorage.getItem("token") === null) {
    adminpanelsection.classList.remove('active')
    loginform.classList.add('active')
} else {
    get_all_settings()
    adminpanelsection.classList.add('active')
    headerusername.innerHTML = JSON.parse(localStorage.getItem('userdata')).username
}

loginformbody.addEventListener('submit', (e) => {
    e.preventDefault()
    login(loginusername.value, loginpassword.value)
})

const login = async (username, password) => {
    const response = await fetch(routes.login(), {
        method: 'POST',
        body: `{"username":"${username}","password":"${password}"}`
    }).then(data => data.json()).then(jsondata => jsondata).catch((error) => {
        if (error.message === "Unexpected end of JSON input") {
            console.log("error")
            notification(`Ошибка на сервере: Создайте файл users.json и запишите в него первого пользователя`, "error")
        } else {
            console.log("error")
            notification(`Ошибка на сервере: ${error}`, "error")
        }
    })
    if (response.status === "403") {
        console.log("Неправильный логин или пароль")
        notification("Неправильный логин или пароль", "error")
    }
    if (response.token) {
        notification("Успешно", "success")
        loginform.classList.remove("active")
        localStorage.setItem("token", response.token)
        get_all_settings()
        adminpanelsection.classList.add('active')
        await get_user_data()
        loginusername.value = ""
        loginpassword.value = ""
    }
}

const get_user_data = async () => {
    const response = await fetch(routes.user(), {
        method: "POST",
        body: `{"token":"${localStorage.getItem("token")}"}`
    }).then(data => data.json()).then(jsondata => jsondata).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
    if (response.id) {
        headerusername.innerHTML = response.username
        localStorage.setItem("userdata", JSON.stringify(response))
    }
}

exitbutton.addEventListener('click', () => {
    localStorage.clear()
    loginform.classList.add('active')
    notification('Вы вышли', "success")
    adminpanelsection.classList.remove('active')
})
