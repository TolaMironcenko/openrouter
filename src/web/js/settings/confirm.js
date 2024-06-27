const confirmform = document.querySelector('#confirmform')
const confirmformheader = document.querySelector('#confirmformheader')
const cancelbutton = document.querySelector('#cancel')
const confirmformbuttons = document.querySelector('#confirmformbuttons')

const get_confirm_form = (header, reqfunc) => {
    console.log("confirmform")
    confirmformheader.innerHTML = header
    console.log("confirmformheader")
    confirmform.classList.add('active')
    console.log("activeconfirmform")
    var confirmbutton = document.createElement('button')
    confirmbutton.classList.add('formbutton')
    confirmbutton.id = "confirm"
    confirmbutton.innerHTML = "Confirm"
    confirmbutton.addEventListener('click', () => {
        reqfunc()
        confirmform.classList.remove('active')
    })
    confirmformbuttons.replaceChild(confirmbutton, document.querySelector('#confirm'))
}

cancelbutton.addEventListener('click', () => {
    confirmform.classList.remove('active')
})
