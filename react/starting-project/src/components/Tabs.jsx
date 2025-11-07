export default function Tabs({children, buttons, ButtonWrapper='menu'}){
    return <>
        <ButtonWrapper>
            {buttons}
        </ButtonWrapper>
        {children}
    </>
}