import { EthereumSignedTx } from 'trezor-connect/lib/typescript'
import { HardwareWalletAccount, HardwareWalletResponseCodeType } from './hardware/types'

export enum HardwareCoins {
  FILECOIN = 'f',
  ETH = 'e'
}

export interface SignHardwareTransactionType {
  success: boolean
  error?: string
  deviceError?: HardwareWalletResponseCodeType
}
export interface SignatureVRS {
  v: number
  r: string
  s: string
}
export type HardwareOperationResult = {
  success: boolean
  error?: string
  code?: string | number
}

export type SignHardwareTransactionOperationResult = HardwareOperationResult & {
  payload?: EthereumSignedTx
}

export type SignHardwareMessageOperationResult = HardwareOperationResult & {
  payload?: string
}

export type GetAccountsHardwareOperationResult = HardwareOperationResult & {
  payload?: HardwareWalletAccount[]
}
